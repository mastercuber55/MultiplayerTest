#include <raylib.h>
#define FRAX_IMPL
#include "Scenes/Scenes.hpp"

int main() {

  Frax::Init("MultiplayerTest", {640, 480});

  SceneStart().Run();

  if (!WindowShouldClose())
    SceneGame().Run();

  Frax::Close();
}