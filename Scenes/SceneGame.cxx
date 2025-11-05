#include "Scenes.hpp"
#include <raylib.h>
#include <raymath.h>

SceneGame::SceneGame() {
  Networking = new SceneNet(this);
  InitAndroidControls();
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

  Networking->Update(dt);
}

void SceneGame::Draw() {
  for (auto &plr : Players) {
    if (plr.x != 0 && plr.y != 0) {
      DrawRectangleV(plr, {32, 32}, BLUE);
    }
  }

  DrawRectangleV(Me, {32, 32}, RED);

  Networking->Draw();
}

SceneGame::~SceneGame() {}