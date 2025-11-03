/*******************************************************************************************
*
*   StartMenu v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_STARTMENU_IMPLEMENTATION
*       #include "gui_StartMenu.h"
*
*       INIT: GuiStartMenuState state = InitGuiStartMenu();
*       DRAW: GuiStartMenu(&state);
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

#include "GuiBase.h"
#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_STARTMENU_H
#define GUI_STARTMENU_H

struct GuiStartMenu : GuiBase {

  bool ServerNameInputEditMode;
  char ServerNameInputText[128];
  bool MaxPlayersInputEditMode;
  int MaxPlayersInputValue;
  bool HostNameInputEditMode;
  char HostNameInputText[128];
  bool PortInputEditMode;
  char PortInputText[128];
  bool LanGameChecked;
  bool HostBtnPressed;
  bool JoinBtnPressed;

  void Init() override;
  void Draw() override;
};

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#ifdef __cplusplus
}
#endif

#endif // GUI_STARTMENU_H

/***********************************************************************************
*
*   GUI_STARTMENU IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_STARTMENU_IMPLEMENTATION)

#include "raygui.h"
//----------------------------------------------------------------------------------
void GuiStartMenu::Init() {

  Window = Rectangle{312, 192, 360, 288};

  ServerNameInputEditMode = false;
  strcpy(ServerNameInputText, "Honeywale");
  MaxPlayersInputEditMode = false;
  MaxPlayersInputValue = 4;
  HostNameInputEditMode = false;
  strcpy(HostNameInputText, "coolgameserver.org");
  PortInputEditMode = false;
  strcpy(PortInputText, "7777");
  LanGameChecked = false;
  HostBtnPressed = false;
  JoinBtnPressed = false;
  Active = true;
  Dragging = false;

}

void GuiStartMenu::Draw()
{
    if (Active)
    {
        Active = !GuiWindowBox(Rectangle{ Window.x + 0, Window.y + 0, Window.width, Window.height }, "SAMPLE TEXT");
        GuiGroupBox(Rectangle{ Window.x + 24, Window.y + 48, 312, 72 }, "#206#Host");
        GuiGroupBox(Rectangle{ Window.x + 24, Window.y + 144, 312, 72 }, "#207#Join");
        if (GuiTextBox(Rectangle{ Window.x + 48, Window.y + 72, 117, 25 }, ServerNameInputText, 128, ServerNameInputEditMode)) ServerNameInputEditMode = !ServerNameInputEditMode;
        GuiLabel(Rectangle{ Window.x + 48, Window.y + 56, 120, 24 }, "Server Name:");
        if (GuiSpinner(Rectangle{ Window.x + 192, Window.y + 72, 120, 24 }, NULL, &MaxPlayersInputValue, 0, 100, MaxPlayersInputEditMode)) MaxPlayersInputEditMode = !MaxPlayersInputEditMode;
        GuiLabel(Rectangle{ Window.x + 192, Window.y + 56, 120, 24 }, "Max Players:");
        if (GuiTextBox(Rectangle{ Window.x + 48, Window.y + 168, 264, 24 }, HostNameInputText, 128, HostNameInputEditMode)) HostNameInputEditMode = !HostNameInputEditMode;
        if (GuiTextBox(Rectangle{ Window.x + 120, Window.y + 240, 96, 24 }, PortInputText, 128, PortInputEditMode)) PortInputEditMode = !PortInputEditMode;
        GuiLabel(Rectangle{ Window.x + 120, Window.y + 224, 120, 24 }, "Port");
        GuiLabel(Rectangle{ Window.x + 48, Window.y + 152, 120, 24 }, "Hostname/IP:");
        GuiCheckBox(Rectangle{ Window.x + 24, Window.y + 240, 24, 24 }, "LAN Game", &LanGameChecked);
        GuiLabel(Rectangle{ Window.x + 48, Window.y + 192, 264, 16 }, "[NOTE] Not required for LAN Games.");
        HostBtnPressed = GuiButton(Rectangle{ Window.x + 240, Window.y + 224, 96, 24 }, "Host"); 
        JoinBtnPressed = GuiButton(Rectangle{ Window.x + 240, Window.y + 256, 96, 24 }, "Join"); 
    }
}

#endif // GUI_STARTMENU_IMPLEMENTATION
