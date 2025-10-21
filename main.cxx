#include "FraxNet.hpp"
#include "enet/enet.h"
#include <cstring>
#include <raylib.h>
#include <unordered_map>

std::unordered_map<ENetPeer *, int> IndexByPeer;
Vector2 Players[4];

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

int main(int argc, char *argv[]) {

  if (argc > 1 && strcmp(argv[1], "host") == 0) {
    Fnet::CreateServer();
  } else {
    Fnet::JoinServer();
  }

  InitWindow(640, 480, "Multiplayer Test");
  Vector2 Me;

  while (!WindowShouldClose()) {
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

            Players[IndexByPeer[Fnet::Event.peer]] = clientPos;
          }
        } else {
          if (Fnet::Event.packet->dataLength == sizeof(Players)) {
            memcpy(Players, Fnet::Event.packet->data, sizeof(Players));
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
      Fnet::SendPacket(Fnet::Peer, &Me, sizeof(Me));
    } else {
      Players[0] = Me;
      for (size_t i = 0; i < Fnet::Host->peerCount; ++i) {
        ENetPeer *peer = &Fnet::Host->peers[i];
        if (peer->state == ENET_PEER_STATE_CONNECTED) {
          Fnet::SendPacket(peer, &Players, sizeof(Players));
        }
      }
    }

    auto spd = 256 * GetFrameTime();

    if (IsKeyDown(KEY_W))
      Me.y += -spd;
    if (IsKeyDown(KEY_A))
      Me.x += -spd;
    if (IsKeyDown(KEY_S))
      Me.y += +spd;
    if (IsKeyDown(KEY_D))
      Me.x += +spd;

    BeginDrawing();
    ClearBackground(BLACK);

    for (auto &plr : Players) {
      if (plr.x != 0 && plr.y != 0) {
        DrawRectangleV(plr, {32, 32}, BLUE);
      }
    }

    DrawRectangleV(Me, {32, 32}, RED);

    EndDrawing();
  }

  CloseWindow();
  Fnet::Stop();
}