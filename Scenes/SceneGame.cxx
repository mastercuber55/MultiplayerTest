#include "Scenes.hpp"

SceneGame::SceneGame() {
  Networking = new SceneFNet(this);
}

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

  Networking->Update(dt);
}

void SceneGame::Draw() {
  for (auto &plr : Players) {
    if (plr.x != 0 && plr.y != 0) {
      DrawRectangleV(plr, {32, 32}, BLUE);
    }
  }

  DrawRectangleV(Me, {32, 32}, RED);
}

SceneGame::~SceneGame() {
  free(Networking);
}