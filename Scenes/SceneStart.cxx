#include "GuiBase.h"
#include "udp_discovery_ip_port.hpp"
#include "udp_discovery_peer.hpp"
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

  // Update all GUI windows
  GuiUpdateWindows(GuiStates);

  // --- Handle Host Button ---
  if (StartMenu.HostBtnPressed) {
    int port = std::stoi(StartMenu.PortInputText);
    int maxPlayers = StartMenu.MaxPlayersInputValue;

    GameNet::CreateServer(port, maxPlayers);

    if (StartMenu.LanGameChecked) {
      GameLAN::InitServer(StartMenu.ServerNameInputText);
      LanMenu.Init();
      LanMenu.WindowName = "Connnected LAN Clients";
      GuiStates.push_back(&LanMenu);
    }
  }

  // --- Handle Join Button ---
  else if (StartMenu.JoinBtnPressed) {
    int port = std::stoi(StartMenu.PortInputText);

    if (!StartMenu.LanGameChecked) {
      GameNet::JoinServer(StartMenu.HostNameInputText, port);
    } else {
      GameLAN::InitClient();
      LanMenu.Init();
      LanMenu.WindowName = "Available LAN Servers";
      GuiStates.push_back(&LanMenu);
    }
  }

  if (!GameLAN::IsServer()) {
    std::list<GameLAN::DPeer> NewDiscoveredPeers = GameLAN::DiscoverAsClient();
    if (!NewDiscoveredPeers.empty() &&
        !udpdiscovery::Same(GameLAN::params.same_peer_mode(), DiscoveredPeers,
                            NewDiscoveredPeers)) {
      IpAddresses.clear(); // Clear because we have discovered new clients.
      for (udpdiscovery::DiscoveredPeer &peer : NewDiscoveredPeers) {
        IpAddresses.push_back(udpdiscovery::IpToString(peer.ip_port().ip()));
        if (LanMenu.options == "")
          LanMenu.options = "";
        LanMenu.options.append(peer.user_data() + ";");
      }
      DiscoveredPeers = NewDiscoveredPeers;
    }
  }

  if (LanMenu.Active && !LanMenu.options.empty() &&
      LanMenu.ListView003Active >= 0) {
    LanMenu.ip = IpAddresses[LanMenu.ListView003Active];
  }

  // --- Exit Condition ---
  if (!StartMenu.Active && !LanMenu.Active) {
    Frax::KeepRunning = false;
  }
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