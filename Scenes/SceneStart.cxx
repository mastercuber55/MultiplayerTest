#include <raylib.h>
#define GUI_STARTMENU_IMPLEMENTATION
#include "Scenes.hpp"
#ifdef PLATFORM_ANDROID
#include <raymob.h>
#endif

SceneStart::SceneStart() {

  GuiLoadStyle("../external/raygui/styles/terminal/style_terminal.rgs");
  StartMenuState = InitGuiStartMenu();

  BackgroundColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void SceneStart::Update(float dt) {
  (void)dt;

  Frax::GuiMakeMoveableWindow(
      StartMenuState.WindowDragging, StartMenuState.Anchor, 360);

  if (StartMenuState.HostBtnPressed) {
    GameNet::CreateServer(std::stoi(StartMenuState.PortInputText),
                          StartMenuState.MaxPlayersInputValue);

    if (StartMenuState.LanGameChecked) {
      GameLAN::InitServer(StartMenuState.ServerNameInputText);
    }
    }

    // if (HostPressed) {
    //   GameNet::CreateServer(static_cast<enet_uint16>(std::stoi(PortText)));
    //   GameLAN::InitServer("Intel-PC");
    //   this->KeepRunning = false;
    // } else if (JoinPressed) {
    //   GameNet::JoinServer(HostnameText,
    //                       static_cast<enet_uint16>(std::stoi(PortText)));
    //   GameLAN::InitClient();
    //   this->KeepRunning = false;
    // }
    if (!StartMenuState.WindowActive)
      Frax::KeepRunning = false;
}

void SceneStart::Draw() {

  GuiStartMenu(&StartMenuState);
}

SceneStart::~SceneStart() {
  GameNet::Stop();
}