#include "Scenes.hpp"
#define FNET_IMPL
#include "../FraxNet.hpp"
#include <cstring>
#include <unordered_map>

std::unordered_map<ENetPeer *, int> IndexByPeer;

int nextAvailableSlot() {
  for (int i = 1; i < 4; ++i) { // 0 reserved for server
    bool used = false;
    for (auto &[peer, idx] : IndexByPeer)
      if (idx == i)
        used = true;
    if (!used)
      return i;
  }
  return -1; // no free slot
}

SceneFNet::SceneFNet(SceneGame* ptr) {
  Parent = ptr;
}

void SceneFNet::Update(float dt) {
  (void)dt;

  while (Fnet::PollEvents(0) > 0) {
    switch (Fnet::Event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
      std::cout << "Client connected.\n";
      int slot = nextAvailableSlot();
      if (slot != -1)
        IndexByPeer[Fnet::Event.peer] = slot;
      break;
    }
    case ENET_EVENT_TYPE_RECEIVE: {
      if (!Fnet::Peer) {
        // Server: receiving a single client's position
        if (Fnet::Event.packet->dataLength == sizeof(Vector2)) {
          Vector2 clientPos;
          memcpy(&clientPos, Fnet::Event.packet->data, sizeof(Vector2));

          Parent->Players[IndexByPeer[Fnet::Event.peer]] = clientPos;
        }
      } else {
        if (Fnet::Event.packet->dataLength == sizeof(Parent->Players)) {
          memcpy(Parent->Players, Fnet::Event.packet->data, sizeof(Parent->Players));
        }
      }
      break;
    }
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "Client disconnected.\n";
      IndexByPeer.erase((Fnet::Event.peer));
      break;
    default:
      break;    
    }
  }

  if (Fnet::Peer) {
    Fnet::SendPacket(Fnet::Peer, &Parent->Me, sizeof(Parent->Me));
  } else {
    Parent->Players[0] = Parent->Me;
    for (size_t i = 0; i < Fnet::Host->peerCount; ++i) {
      ENetPeer *peer = &Fnet::Host->peers[i];
      if (peer->state == ENET_PEER_STATE_CONNECTED) {
        Fnet::SendPacket(peer, &Parent->Players, sizeof(Parent->Players));
      }
    }
  }
}

void SceneFNet::Draw() {}

SceneFNet::~SceneFNet() { Fnet::Stop(); }