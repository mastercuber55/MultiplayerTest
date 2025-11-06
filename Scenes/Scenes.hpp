#include "../GameNet.hpp"
#include "GuiLanMenu.h"
#include "GuiStartMenu.h"
#include <Frax.hpp>
#include <raylib.h>
#include <unordered_map>
#include <vector>

struct SceneGame : Frax::Scene {

  Vector2 Me = { 0, 0};
  std::vector<Vector2> Others;

  Vector2 joyBase;
  Vector2 joyValue;
  Vector2 joyKnob;
  float joyRadius;

  SceneGame(std::any names);

  void Update(float dt) override;
  void Draw() override;

  void InitAndroidControls();
  void UpdateAndroidControls();
  void DrawAndroidControls();

  void HandleNet();
  void NetSendData();
  void NetOnConnect();
  void NetOnDisconnect();
  void NetOnRecieve();

  ~SceneGame();
};
struct SceneStart : Frax::Scene {

  GuiStartMenu StartMenu;
  GuiLanMenu LanMenu;
  std::list<GameLAN::DPeer> DiscoveredPeers;
  std::vector<std::string> IpAddresses; // Client Side
  std::unordered_map<ENetPeer *, std::string> Names; // Server Side

  bool isConnected;
  bool isHost;

  SceneStart();

  void Update(float dt) override;
  void Draw() override;

  void HandleButtons();
  void HandleNet();

  void HandleHostButtonPressed();
  void HandleJoinButtonPressed();
  void HandleLanMenuUpdates();

  void HandleLanDiscovery();

  void HandleConnect();
  void HandleDisconnect();
  void HandleRecieve();

  ~SceneStart();
};