#include <twm.hpp>
#include <string>
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LANMENU_H
#define GUI_LANMENU_H

struct GuiLanMenu : TWM::Window {

  bool ButtonPressed;
  std::string ButtonText;
  int ListView003ScrollIndex;
  int ListView003Active;
  const char *WindowName;
  std::string options;
  std::string ip;

  void Init() override;
  void Draw() override;
};


#endif // GUI_LANMENU_H

/***********************************************************************************
*
*   GUI_LANMENU IMPLEMENTATION
*
************************************************************************************/
// #define GUI_LANMENU_IMPLEMENTATION
#if defined(GUI_LANMENU_IMPLEMENTATION)

#include "raygui.h"

void GuiLanMenu::Init(void)
{
  
  Bounds = Rectangle{0, 0, 432, 264};

  Active = true;
  WindowName = "DYNAMIC NAME";
  Dragging = false;
  ButtonPressed = false;
  ListView003ScrollIndex = 0;
  ListView003Active = 0;

  TWM::States.push_back(this);

}

void GuiLanMenu::Draw()
{
    if (Active)
    {
        Active = !GuiWindowBox(Bounds, WindowName);
        ButtonPressed = GuiButton(Rectangle{ Bounds.x + 288, Bounds.y + 216, 120, 24 }, ButtonText.c_str()); 
        GuiListView(Rectangle{ Bounds.x + 24, Bounds.y + 48, 384, 152 }, options.c_str(), &ListView003ScrollIndex, &ListView003Active);
        GuiStatusBar(Rectangle{ Bounds.x + 24, Bounds.y + 216, 240, 24 }, ip.c_str());
        GuiLabel(Rectangle{ Bounds.x + 24, Bounds.y + 200, 120, 24 }, "Ip Address");
    }
}

#endif // GUI_LANMENU_IMPLEMENTATION
