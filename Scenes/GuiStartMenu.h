#include <twm.hpp>
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_STARTMENU_H
#define GUI_STARTMENU_H

struct GuiStartMenu : TWM::Window {

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

  Bounds = Rectangle{312, 192, 360, 288};

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

  TWM::States.push_back(this);
}

void GuiStartMenu::Draw()
{
    if (Active)
    {
        Active = !GuiWindowBox(Bounds, "Start A Game");
        GuiGroupBox(Rectangle{ Bounds.x + 24, Bounds.y + 48, 312, 72 }, "#206#Host");
        GuiGroupBox(Rectangle{ Bounds.x + 24, Bounds.y + 144, 312, 72 }, "#207#Join");
        if (GuiTextBox(Rectangle{ Bounds.x + 48, Bounds.y + 72, 117, 25 }, ServerNameInputText, 128, ServerNameInputEditMode)) ServerNameInputEditMode = !ServerNameInputEditMode;
        GuiLabel(Rectangle{ Bounds.x + 48, Bounds.y + 56, 120, 24 }, "Server Name:");
        if (GuiSpinner(Rectangle{ Bounds.x + 192, Bounds.y + 72, 120, 24 }, NULL, &MaxPlayersInputValue, 0, 100, MaxPlayersInputEditMode)) MaxPlayersInputEditMode = !MaxPlayersInputEditMode;
        GuiLabel(Rectangle{ Bounds.x + 192, Bounds.y + 56, 120, 24 }, "Max Players:");
        if (GuiTextBox(Rectangle{ Bounds.x + 48, Bounds.y + 168, 264, 24 }, HostNameInputText, 128, HostNameInputEditMode)) HostNameInputEditMode = !HostNameInputEditMode;
        if (GuiTextBox(Rectangle{ Bounds.x + 120, Bounds.y + 240, 96, 24 }, PortInputText, 128, PortInputEditMode)) PortInputEditMode = !PortInputEditMode;
        GuiLabel(Rectangle{ Bounds.x + 120, Bounds.y + 224, 120, 24 }, "Port");
        GuiLabel(Rectangle{ Bounds.x + 48, Bounds.y + 152, 120, 24 }, "Hostname/IP:");
        GuiCheckBox(Rectangle{ Bounds.x + 24, Bounds.y + 240, 24, 24 }, "LAN Game", &LanGameChecked);
        GuiLabel(Rectangle{ Bounds.x + 48, Bounds.y + 192, 264, 16 }, "[NOTE] Not required for LAN Games.");
        HostBtnPressed = GuiButton(Rectangle{ Bounds.x + 240, Bounds.y + 224, 96, 24 }, "Host"); 
        JoinBtnPressed = GuiButton(Rectangle{ Bounds.x + 240, Bounds.y + 256, 96, 24 }, "Join"); 
    }
}

#endif // GUI_STARTMENU_IMPLEMENTATION
