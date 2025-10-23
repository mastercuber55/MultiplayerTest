#include "Scenes.hpp"
#define RAYGUI_IMPLEMENTATION
extern "C" {
#include "../external/raygui/src/raygui.h"
}
#include "../FraxNet.hpp"

SceneStart::SceneStart() {
  GuiLoadStyle("../external/raygui/styles/cherry/style_cherry.rgs");
}

void SceneStart::Update(float dt) {
  (void)dt;
  if (HostPressed) {
    Fnet::CreateServer();
    this->KeepRunning = false;
  } else if (JoinPressed) {
    Fnet::JoinServer();
    this->KeepRunning = false;
  }
}

void SceneStart::Draw() {
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  // raygui: controls drawing
  //----------------------------------------------------------------------------------
  HostPressed =
      GuiButton({anchor01.x + 144, anchor01.y + 136, 120, 24}, "Host");
  JoinPressed = GuiButton({anchor01.x + 144, anchor01.y + 96, 120, 24}, "Join");
  if (GuiTextBox({anchor01.x + 24, anchor01.y + 24, 240, 56}, HostnameText, 128,
                 HostnameEditMode))
    HostnameEditMode = !HostnameEditMode;
  GuiGroupBox({anchor01.x + 0, anchor01.y + 0, 288, 184}, "Join Or Host Game");
  GuiLabel({anchor01.x + 24, anchor01.y + 80, 120, 24}, "Port:");
  GuiLabel({anchor01.x + 24, anchor01.y + 8, 120, 24}, "Hostname:");
  if (GuiTextBox({anchor01.x + 24, anchor01.y + 96, 112, 64}, TextBox006Text,
                 128, TextBox006EditMode))
    TextBox006EditMode = !TextBox006EditMode;
  //----------------------------------------------------------------------------------
}

SceneStart::~SceneStart() {}