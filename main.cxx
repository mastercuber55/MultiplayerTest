#define FRAX_IMPL
#include "Scenes/Scenes.hpp"

int main(int argc, char *argv[]) {

  Frax::Init("MultiplayerTest", {640, 480});

  SceneGame(argc, argv).Run();

  Frax::Close();
}