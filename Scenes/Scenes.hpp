#include "../GameNet.hpp"
#include "GuiLanMenu.h"
#include "GuiStartMenu.h"
#include <Frax.hpp>
#include <vector>

struct SceneNet;

struct SceneGame : Frax::Scene {

  Vector2 Me = { 340, 200};
  Vector2 Players[4];

  SceneNet *Networking;

  Vector2 joyBase;
  Vector2 joyValue;
  Vector2 joyKnob;
  float joyRadius;

  SceneGame();

  void Update(float dt) override;
  void Draw() override;

  ~SceneGame();
};

struct SceneNet : Frax::Scene {

  SceneGame *Parent;
  SceneNet(SceneGame *ptr);

  void Update(float dt) override;
  void Draw() override;

  ~SceneNet();
};

struct SceneStart : Frax::Scene {

  GuiStartMenu StartMenu;
  GuiLanMenu LanMenu;
  std::list<GameLAN::DPeer> DiscoveredPeers;
  std::vector<std::string> IpAddresses;

  bool isHost;

  SceneStart();

  void Update(float dt) override;
  void Draw() override;

  ~SceneStart();
};