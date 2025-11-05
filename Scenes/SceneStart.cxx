#define TWM_IMPLEMENTATION
#include "enet/enet.h"
#include <iostream>
#include <raylib.h>
#include <twm.hpp>
#define GUI_STARTMENU_IMPLEMENTATION
#define GUI_LANMENU_IMPLEMENTATION
#include "Scenes.hpp"
#ifdef PLATFORM_ANDROID
#include <raymob.h>
#endif

SceneStart::SceneStart() {

  GuiLoadStyle("../external/raygui/styles/terminal/style_terminal.rgs");
  StartMenu.Init();

  BackgroundColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void SceneStart::Update(float dt) {
  (void)dt;

  TWM::Update();

  // --- Handle Host Button ---
  if (StartMenu.HostBtnPressed) {
    int port = std::stoi(StartMenu.PortInputText);
    int maxPlayers = StartMenu.MaxPlayersInputValue;

    GameNet::CreateServer(port, maxPlayers);

    if (StartMenu.LanGameChecked) {
      GameLAN::InitServer(StartMenu.ServerNameInputText);
      LanMenu.Init();
      LanMenu.ButtonText = "Start";
      LanMenu.WindowName = "Connnected LAN Clients";
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
      LanMenu.ButtonText = "Join";
      LanMenu.WindowName = "Available LAN Servers";
    }
  }

  if (LanMenu.ButtonPressed && LanMenu.ListView003Active >= 0 &&
      !isConnected && !LanMenu.ip.empty()) {
    GameNet::JoinServer(LanMenu.ip.c_str(), std::stoi(StartMenu.PortInputText));
  }

  if (!GameLAN::IsServer()) {
    std::list<GameLAN::DPeer> NewDiscoveredPeers = GameLAN::DiscoverAsClient();
    if (!NewDiscoveredPeers.empty() &&
        !udpdiscovery::Same(GameLAN::params.same_peer_mode(), DiscoveredPeers,
                            NewDiscoveredPeers)) {
      IpAddresses.clear(); // Clear because we have discovered new clients.
      for (udpdiscovery::DiscoveredPeer &peer : NewDiscoveredPeers) {
        IpAddresses.push_back(udpdiscovery::IpToString(peer.ip_port().ip()));
        LanMenu.options.append(peer.user_data() + ";");
      }
      LanMenu.options.pop_back();
      DiscoveredPeers = NewDiscoveredPeers;
    }
  }

  if (GameNet::Host) {
    while (GameNet::PollEvents(0) > 0) {
      switch (GameNet::Event.type) {
      case ENET_EVENT_TYPE_CONNECT: {
        isConnected = true;
        if (GameNet::Peer) {
          LanMenu.ButtonText = "Connected";
          std::string name = "Cube Nerd";
          GameNet::SendPacket(GameNet::Peer, name.c_str(), name.size() + 1);
        }
        break;
      }
      case ENET_EVENT_TYPE_DISCONNECT: {
        if (GameNet::Peer) {
          LanMenu.ButtonText = "Join";
        } else {
          Names.erase(GameNet::Event.peer);
          LanMenu.options.clear();
          for (const auto &[peer, name] : Names) {
            LanMenu.options.append(name + ";");
          }
          LanMenu.options.pop_back();
        }
        break;
      }
      case ENET_EVENT_TYPE_RECEIVE: {
        if (!GameNet::Peer) {
          std::string name((char *)GameNet::Event.packet->data,
                           GameNet::Event.packet->dataLength);
          LanMenu.options.append(name + ";  ");
          Names[GameNet::Event.peer] = name;
        }
        break;
      }
      case ENET_EVENT_TYPE_NONE: {
        break;
      }
      }
    }
  }

  if (LanMenu.Active && !GameLAN::IsServer() && !LanMenu.options.empty() &&
      LanMenu.ListView003Active >= 0) {
    LanMenu.ip = IpAddresses[LanMenu.ListView003Active];
  }

  // --- Exit Condition ---
  if (!StartMenu.Active && !LanMenu.Active) {
    Frax::KeepRunning = false;
  }
}

void SceneStart::Draw() { TWM::Draw(); }

SceneStart::~SceneStart() { GameNet::Stop(); }