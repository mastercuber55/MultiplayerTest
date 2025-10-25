#include "Scenes.hpp"
#define RAYGUI_IMPLEMENTATION
extern "C" {
#include "../external/raygui/src/raygui.h"
}
#ifdef PLATFORM_ANDROID
#include <raymob.h>
#endif

SceneStart::SceneStart() {
#ifndef PLATFORM_ANDROID
  GuiLoadStyle("../external/raygui/styles/terminal/style_terminal.rgs");
#else
  GuiLoadStyle("styles/terminal/style_terminal.rgs");
#endif
}

void SceneStart::Update(float dt) {
  (void)dt;
  
  if (HostPressed) {
    GameNet::CreateServer(static_cast<enet_uint16>(std::stoi(PortText)));
    GameLAN::InitServer("Intel-PC");
    this->KeepRunning = false;
  } else if (JoinPressed) {
    GameNet::JoinServer(HostnameText,
                        static_cast<enet_uint16>(std::stoi(PortText)));
    GameLAN::InitClient();
    this->KeepRunning = false;
  }
}

void SceneStart::Draw() {
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  HostPressed =
      GuiButton({anchor01.x + 144, anchor01.y + 136, 120, 24}, "Host");
  JoinPressed = GuiButton({anchor01.x + 144, anchor01.y + 96, 120, 24}, "Join");

  if (GuiTextBox({anchor01.x + 24, anchor01.y + 24, 240, 56}, HostnameText, 128,
                 HostnameEditMode)) {
    HostnameEditMode = !HostnameEditMode;
#ifdef PLATFORM_ANDROID
    if (HostnameEditMode)
      ShowSoftKeyboard();
#endif
  }

  GuiGroupBox({anchor01.x + 0, anchor01.y + 0, 288, 184}, "Join Or Host Game");
  GuiLabel({anchor01.x + 24, anchor01.y + 80, 120, 24}, "Port:");
  GuiLabel({anchor01.x + 24, anchor01.y + 8, 120, 24}, "Hostname:");

  if (GuiTextBox({anchor01.x + 24, anchor01.y + 96, 112, 64}, PortText, 128,
                 PortEditMode)) {
    PortEditMode = !PortEditMode;
#ifdef PLATFORM_ANDROID
    if (PortEditMode)
      ShowSoftKeyboard();
#endif
  }

#ifdef PLATFORM_ANDROID
  if (HostnameEditMode)
    SoftKeyboardEditText(HostnameText, 128);
  else if (PortEditMode)
    SoftKeyboardEditText(PortText, 128);
  else
    HideSoftKeyboard();
#endif
}

SceneStart::~SceneStart() {}