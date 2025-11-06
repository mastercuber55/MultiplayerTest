#include "Scenes.hpp"
#include "enet/enet.h"
#include <any>
#include <cstring>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>

struct PeerData {
  std::string Name;
  size_t indexOfPlayer;
};

SceneGame::SceneGame(std::any names) {
  InitAndroidControls();
  auto Names =
      std::any_cast<std::unordered_map<ENetPeer *, std::string>>(names);
  if (Names.empty())
    return;

  Others.push_back(Me);
  
  size_t i = 1;
  for (const auto &[peer, name] : Names) {
    auto data = new PeerData;
    data->Name = name;
    data->indexOfPlayer = i;
    peer->data = data;
    i++;
  }
}
#ifdef PLATFORM_ANDORID
void SceneGame::InitAndroidControls() {
  joyRadius = 64;
  joyBase = {static_cast<float>(joyRadius * 1.5),
             Frax::ScreenSize.y - joyRadius * 2};
  joyKnob = joyBase;
  joyValue = {0, 0};
}
void SceneGame::UpdateAndroidControls() {
  int touchCount = GetTouchPointCount();
  bool joystickActive = false;

  for (int i = 0; i < touchCount; i++) {
    Vector2 touch = GetTouchPosition(i);

    // Allows joystick if touch is detected just near the joystick.
    if (CheckCollisionCircles(joyBase, joyRadius, touch, joyRadius * 2)) {
      joystickActive = true;

      // calculate delta from base
      Vector2 diff = Vector2Subtract(touch, joyBase);
      float len = Vector2Length(diff);

      // clamp to joystick radius
      if (len > joyRadius)
        diff = Vector2Scale(Vector2Normalize(diff), joyRadius);

      // update knob position
      joyKnob = Vector2Add(joyBase, diff);

      // normalized analog value [-1,1]
      joyValue = Vector2Scale(diff, 1.0f / joyRadius);
    }
  }

  // reset joystick if no touch in left half
  if (!joystickActive) {
    joyKnob = joyBase;
    joyValue = {0, 0};
  }

  Me.x += joyValue.x * spd;
  Me.y += joyValue.y * spd;
}
void SceneGame::DrawAndroidControls() {
  DrawCircleLinesV(joyBase, joyRadius, WHITE);
  DrawCircleV(joyKnob, joyRadius / 2, WHITE);
}
#else
void SceneGame::InitAndroidControls() {}
void SceneGame::UpdateAndroidControls() {}
void SceneGame::DrawAndroidControls() {}
#endif
void SceneGame::Update(float dt) {
  auto spd = 256 * dt;

  if (IsKeyDown(KEY_W))
    Me.y += -spd;
  if (IsKeyDown(KEY_A))
    Me.x += -spd;
  if (IsKeyDown(KEY_S))
    Me.y += +spd;
  if (IsKeyDown(KEY_D))
    Me.x += +spd;

  HandleNet();
}

void SceneGame::HandleNet() {
  while (GameNet::PollEvents(0) > 0) {
    switch (GameNet::Event.type) {
    case ENET_EVENT_TYPE_CONNECT:
      NetOnConnect();
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      NetOnDisconnect();
      break;
    case ENET_EVENT_TYPE_RECEIVE:
      NetOnRecieve();
      break;
    case ENET_EVENT_TYPE_NONE: {
    } break;
    }
  }

  NetSendData();
}

void SceneGame::NetSendData() {
  if (GameNet::Peer) {
    GameNet::SendPacket(GameNet::Peer, &Me, sizeof(Vector2));
  } else {
    Others[0] = Me;
    for (size_t i = 0; i < GameNet::Host->peerCount; ++i) {
      ENetPeer *peer = &GameNet::Host->peers[i];
      if (peer->state == ENET_PEER_STATE_CONNECTED) {

        GameNet::SendPacket(peer, Others.data(),
                            sizeof(Vector2) * Others.size());
      }
    }
  }
}
void SceneGame::NetOnConnect() {}
void SceneGame::NetOnDisconnect() {
  delete (PeerData *)GameNet::Event.peer->data;
}
void SceneGame::NetOnRecieve() {
  if (GameNet::Peer) {
    size_t DataSize = GameNet::Event.packet->dataLength;
    size_t Count = DataSize / sizeof(Vector2);

    Others.resize(Count);
    std::memcpy(Others.data(), GameNet::Event.packet->data, DataSize);
  } else {
    Vector2 Player = *(Vector2 *)GameNet::Event.packet->data;
    auto data = (PeerData *)GameNet::Event.peer->data;
    if (data->indexOfPlayer >= Others.size())
      Others.resize(data->indexOfPlayer + 1);
    Others[data->indexOfPlayer] = Player;
  }
}

void SceneGame::Draw() {
  for (auto &plr : Others) {
    DrawRectangleV(plr, {32, 32}, BLUE);
  }

  DrawRectangleV(Me, {32, 32}, RED);
}

SceneGame::~SceneGame() {}