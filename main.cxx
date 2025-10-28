#include <raylib.h>
#define FRAX_IMPL
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "Scenes/Scenes.hpp"

int main() {

  Frax::Init("MultiplayerTest", {640, 480});

  SceneStart().Run();

  if (!Frax::ShouldClose())
    SceneGame().Run();

  Frax::Close();
}