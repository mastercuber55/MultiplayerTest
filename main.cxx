#define FRAX_IMPL
#define RAYGUI_IMPLEMENTATION
#define GAMENET_IMPL
#include <raygui.h>
#include "Scenes/Scenes.hpp"

int main() {

  Frax::Init("MultiplayerTest", {640, 480});

  auto Names = SceneStart().Run();
  
  if (!Frax::ShouldClose())
    SceneGame(Names).Run();

  Frax::Close();
}