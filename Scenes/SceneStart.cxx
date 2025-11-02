#include "GuiBase.h"
#include <iostream>
#include <raylib.h>
#define GUI_STARTMENU_IMPLEMENTATION
#define GUI_LANMENU_IMPLEMENTATION
#include "Scenes.hpp"
#ifdef PLATFORM_ANDROID
#include <raymob.h>
#endif

void GuiUpdateWindows(std::vector<GuiBase *> &GuiStates) {
  Vector2 Mouse = GetMousePosition();

  for (int i = (int)GuiStates.size() - 1; i >= 0; i--) {
    GuiBase *win = GuiStates[i];

    if (win->Active)
      Frax::GuiMakeMoveableWindow(win->Dragging, win->Window);
    else
      continue;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(Mouse, GuiStates[i]->Window)) {
      GuiStates.erase(GuiStates.begin() + i);
      GuiStates.push_back(win);
      break;
    }
  }
}

SceneStart::SceneStart() {

  GuiLoadStyle("../external/raygui/styles/terminal/style_terminal.rgs");
  StartMenu.Init();

  GuiStates.push_back(&StartMenu);

  BackgroundColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void SceneStart::Update(float dt) {
  (void)dt;

  GuiUpdateWindows(GuiStates);

  if (StartMenu.HostBtnPressed) {
    GameNet::CreateServer(std::stoi(StartMenu.PortInputText),
                          StartMenu.MaxPlayersInputValue);

    if (StartMenu.LanGameChecked) {
      GameLAN::InitServer(StartMenu.ServerNameInputText);
      LanMenu.Init();
      GuiStates.push_back(&LanMenu);
    }
  } else if (StartMenu.JoinBtnPressed) {
    if (!StartMenu.LanGameChecked) {
      GameNet::JoinServer(StartMenu.HostNameInputText,
                          std::stoi(StartMenu.PortInputText));
    } else {
      LanMenu.Init();
      GuiStates.push_back(&LanMenu);
      GameLAN::InitClient();
    }
  }

  if (!StartMenu.Active)
    Frax::KeepRunning = false;
}

void SceneStart::Draw() {
  if (GuiStates.size() > 1) {
    GuiLock();
    for (size_t i = 0; i < GuiStates.size() - 1; i++) {
      GuiStates[i]->Draw();
    }
    GuiUnlock();
  }
  GuiStates.back()->Draw();
}

SceneStart::~SceneStart() { GameNet::Stop(); }