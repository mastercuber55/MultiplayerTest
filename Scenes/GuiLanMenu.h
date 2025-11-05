/*******************************************************************************************
*
*   LanMenu v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_LANMENU_IMPLEMENTATION
*       #include "gui_LanMenu.h"
*
*       INIT: GuiLanMenuState state = InitGuiLanMenu();
*       DRAW: GuiLanMenu(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 mastercuber55. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include <twm.hpp>
#include <string>

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LANMENU_H
#define GUI_LANMENU_H

struct GuiLanMenu : TWM::Window {

  bool Button002Pressed;
  int ListView003ScrollIndex;
  int ListView003Active;
  const char *WindowName;
  std::string options;
  std::string ip;

  void Init() override;
  void Draw() override;
};

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#ifdef __cplusplus
}
#endif

#endif // GUI_LANMENU_H

/***********************************************************************************
*
*   GUI_LANMENU IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LANMENU_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void GuiLanMenu::Init(void)
{
  
  Bounds = Rectangle{312, 168, 432, 264};

  Active = true;
  WindowName = "DYNAMIC NAME";
  ip = "192.168.x.x";
  Dragging = false;
  Button002Pressed = false;
  ListView003ScrollIndex = 0;
  ListView003Active = 0;

  TWM::States.push_back(this);

}

void GuiLanMenu::Draw()
{
    if (Active)
    {
        Active = !GuiWindowBox(Bounds, WindowName);
        Button002Pressed = GuiButton(Rectangle{ Bounds.x + 288, Bounds.y + 216, 120, 24 }, "Start/Join"); 
        GuiListView(Rectangle{ Bounds.x + 24, Bounds.y + 48, 384, 152 }, options.c_str(), &ListView003ScrollIndex, &ListView003Active);
        GuiStatusBar(Rectangle{ Bounds.x + 24, Bounds.y + 216, 240, 24 }, ip.c_str());
        GuiLabel(Rectangle{ Bounds.x + 24, Bounds.y + 200, 120, 24 }, "Ip Address");
    }
}

#endif // GUI_LANMENU_IMPLEMENTATION
