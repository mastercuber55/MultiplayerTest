#include <Frax.hpp>

struct SceneFNet;

struct SceneGame : Frax::Scene {

  Vector2 Me = { 340, 200};
  Vector2 Players[4];

  SceneFNet* Networking;

  SceneGame(int argc, char *argv[]);

  void Update(float dt) override;
  void Draw() override;

  ~SceneGame();
};

struct SceneFNet : Frax::Scene {

  SceneGame* Parent;

  SceneFNet(SceneGame *ptr, int argc, char *argv[]);

  void Update(float dt) override;
  void Draw() override;

  ~SceneFNet();
};

struct SceneStart : Frax::Scene {
  SceneStart();

  void Update(float dt) override;
  void Draw() override;

  ~SceneStart();
};