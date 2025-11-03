#include <raylib.h>
#define GAMENET_IMPL
#include "Scenes.hpp"
#include <cstring>
#include <iostream>
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

SceneNet::SceneNet(SceneGame *ptr) { Parent = ptr; }

void SceneNet::Update(float dt) {
  (void)dt;

  while (GameNet::PollEvents(0) > 0) {
    switch (GameNet::Event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
      std::cout << "Client connected.\n";
      int slot = nextAvailableSlot();
      if (slot != -1)
        IndexByPeer[GameNet::Event.peer] = slot;
      break;
    }
    case ENET_EVENT_TYPE_RECEIVE: {
      if (!GameNet::Peer) {
        // Server: receiving a single client's position
        if (GameNet::Event.packet->dataLength == sizeof(Vector2)) {
          Vector2 clientPos;
          memcpy(&clientPos, GameNet::Event.packet->data, sizeof(Vector2));

          Parent->Players[IndexByPeer[GameNet::Event.peer]] = clientPos;
        }
      } else {
        if (GameNet::Event.packet->dataLength == sizeof(Parent->Players)) {
          memcpy(Parent->Players, GameNet::Event.packet->data,
                 sizeof(Parent->Players));
        }
      }
      break;
    }
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "Client disconnected.\n";
      IndexByPeer.erase((GameNet::Event.peer));
      break;
    default:
      break;
    }
  }

  if (GameNet::Peer) {
    GameNet::SendPacket(GameNet::Peer, &Parent->Me, sizeof(Parent->Me));
  } else {
    Parent->Players[0] = Parent->Me;
    for (size_t i = 0; i < GameNet::Host->peerCount; ++i) {
      ENetPeer *peer = &GameNet::Host->peers[i];
      if (peer->state == ENET_PEER_STATE_CONNECTED) {
        GameNet::SendPacket(peer, &Parent->Players, sizeof(Parent->Players));
      }
    }
  }
}

void SceneNet::Draw() {
}

SceneNet::~SceneNet() { GameNet::Stop(); }