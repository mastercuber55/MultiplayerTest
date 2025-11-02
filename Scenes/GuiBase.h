#pragma once
#include <raylib.h>
struct GuiBase {

  Rectangle Window;
  bool Dragging = false;
  bool Active = false;

  virtual void Init() {}
  virtual void Draw() {}
};