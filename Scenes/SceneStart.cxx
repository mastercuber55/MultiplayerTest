#define TWM_IMPLEMENTATION
#include "enet/enet.h"
#include <raylib.h>
#include <twm.hpp>
#define GUI_STARTMENU_IMPLEMENTATION
#define GUI_LANMENU_IMPLEMENTATION
#include "Scenes.hpp"
#ifdef PLATFORM_ANDROID
#include <raymob.h>
#endif

static int GAME_STARTED_CODE = 7;

SceneStart::SceneStart() {

  GuiLoadStyle("../external/raygui/styles/terminal/style_terminal.rgs");
  StartMenu.Init();

  BackgroundColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void SceneStart::Update(float dt) {
  (void)dt;

  TWM::Update();

  HandleButtons();
  HandleNet();

  // --- Exit Condition ---
  if (!StartMenu.Active && !LanMenu.Active) {
    Frax::KeepRunning = false;
  }
}

inline void SceneStart::HandleHostButtonPressed() {
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

inline void SceneStart::HandleJoinButtonPressed() {
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

inline void SceneStart::HandleLanMenuUpdates() {

  // Handle a client trying to join a server.
  if (LanMenu.ListView003Active >= 0 && !isConnected && !LanMenu.ip.empty()) {
    GameNet::JoinServer(LanMenu.ip.c_str(), std::stoi(StartMenu.PortInputText));
  }
  // Handle a server starting the game.
  else if (isConnected && !GameNet::Peer) {
    KeepRunning = false;
    GameNet::BroadcastPacket(&GAME_STARTED_CODE, sizeof(int));
  }

  if (LanMenu.Active && !GameLAN::IsServer() && !LanMenu.options.empty() &&
      LanMenu.ListView003Active >= 0) {
    LanMenu.ip = IpAddresses[LanMenu.ListView003Active];
  }
}

void SceneStart::HandleButtons() {
  if (StartMenu.HostBtnPressed)
    HandleHostButtonPressed();
  else if (StartMenu.JoinBtnPressed)
    HandleJoinButtonPressed();
  else if (LanMenu.ButtonPressed)
    HandleLanMenuUpdates();
}

inline void SceneStart::HandleLanDiscovery() {
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

inline void SceneStart::HandleConnect() {
  isConnected = true;
  if (GameNet::Peer) {
    LanMenu.ButtonText = "Connected";
    std::string name = "Cube Nerd";
    GameNet::SendPacket(GameNet::Peer, name.c_str(), name.size() + 1);
  }
}

inline void SceneStart::HandleDisconnect() {
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
}

inline void SceneStart::HandleRecieve() {
  if (!GameNet::Peer) {
    std::string name((char *)GameNet::Event.packet->data,
                     GameNet::Event.packet->dataLength);
    LanMenu.options.append(name + ";  ");
    Names[GameNet::Event.peer] = name;
  } else {
    if (GameNet::Event.packet->dataLength == sizeof(int)) {
      int code;
      memcpy(&code, GameNet::Event.packet->data, sizeof(int));

      if (code == GAME_STARTED_CODE)
        KeepRunning = false;
    }
  }
}

void SceneStart::HandleNet() {
  if (!GameLAN::IsServer())
    HandleLanDiscovery();

  if (GameNet::Host) {
    while (GameNet::PollEvents(0) > 0) {
      switch (GameNet::Event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        HandleConnect();
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        HandleDisconnect();
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        HandleRecieve();
        break;
      case ENET_EVENT_TYPE_NONE: {
      } break;
      }
    }
  }
}
void SceneStart::Draw() { TWM::Draw(); }

SceneStart::~SceneStart() { GameNet::Stop(); }