#include <Frax.hpp>
#include "../GameNet.hpp"

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

  std::list<udpdiscovery::DiscoveredPeer> peers;

  SceneNet(SceneGame *ptr);

  void Update(float dt) override;
  void Draw() override;

  ~SceneNet();
};

struct SceneStart : Frax::Scene {

  Vector2 anchor01 = {176, 156};

  bool HostPressed = false;
  bool JoinPressed = false;
  bool HostnameEditMode = false;
  char HostnameText[128] = "127.0.0.1";
  bool PortEditMode = false;
  char PortText[128] = "7777";

  bool isHost;

  SceneStart();

  void Update(float dt) override;
  void Draw() override;

  ~SceneStart();
};