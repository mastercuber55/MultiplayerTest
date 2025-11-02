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

#include "raylib.h"
#include "GuiBase.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LANMENU_H
#define GUI_LANMENU_H

struct GuiLanMenu : GuiBase {

  bool Button002Pressed;
  int ListView003ScrollIndex;
  int ListView003Active;

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

  Window = (Rectangle){312, 168, 432, 264};

  Active = true;
  Dragging = false;
  Button002Pressed = false;
  ListView003ScrollIndex = 0;
  ListView003Active = 0;

}

void GuiLanMenu::Draw()
{
    if (Active)
    {
        Active = !GuiWindowBox((Rectangle){ Window.x + 0, Window.y + 0, Window.width, Window.height }, "DYNAMIC NAME");
        Button002Pressed = GuiButton((Rectangle){ Window.x + 288, Window.y + 216, 120, 24 }, "Start/Join"); 
        GuiListView((Rectangle){ Window.x + 24, Window.y + 48, 384, 152 }, "ONE;TWO;THREE;FOUR;FIVE;SIX;SEVEN;EIGHT", &ListView003ScrollIndex, &ListView003Active);
        GuiStatusBar((Rectangle){ Window.x + 24, Window.y + 216, 240, 24 }, "192.168.X.X");
        GuiLabel((Rectangle){ Window.x + 24, Window.y + 200, 120, 24 }, "Ip Address");
    }
}

#endif // GUI_LANMENU_IMPLEMENTATION
