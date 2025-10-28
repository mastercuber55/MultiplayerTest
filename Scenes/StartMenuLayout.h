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
*   Copyright (c) 2022 Cube Nerd. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_STARTMENU_H
#define GUI_STARTMENU_H

typedef struct {
    Vector2 Anchor;

    bool WindowActive;
    bool WindowDragging;
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
    bool TextBox015EditMode;
    char TextBox015Text[128];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiStartMenuState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiStartMenuState InitGuiStartMenu(void);
void GuiStartMenu(GuiStartMenuState *state);

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
GuiStartMenuState InitGuiStartMenu(void)
{
    GuiStartMenuState state = { 0 };

    state.Anchor = (Vector2){ 0, 0 };

    state.WindowActive = true;
    state.WindowDragging = false;
    state.ServerNameInputEditMode = false;
    strcpy(state.ServerNameInputText, "Honeywale");
    state.MaxPlayersInputEditMode = false;
    state.MaxPlayersInputValue = 0;
    state.HostNameInputEditMode = false;
    strcpy(state.HostNameInputText, "coolgameserver.org");
    state.PortInputEditMode = false;
    strcpy(state.PortInputText, "7777");
    state.LanGameChecked = false;
    state.HostBtnPressed = false;
    state.JoinBtnPressed = false;
    state.TextBox015EditMode = false;
    strcpy(state.TextBox015Text, "7777");

    // Custom variables initialization

    return state;
}

void GuiStartMenu(GuiStartMenuState *state)
{
    if (state->WindowActive)
    {
        state->WindowActive = !GuiWindowBox((Rectangle){ state->Anchor.x + 0, state->Anchor.y + 0, 360, 288 }, "#186#Start Game");
        GuiGroupBox((Rectangle){ state->Anchor.x + 24, state->Anchor.y + 48, 312, 72 }, "#206#Host");
        GuiGroupBox((Rectangle){ state->Anchor.x + 24, state->Anchor.y + 144, 312, 72 }, "#207#Join");
        if (GuiTextBox((Rectangle){ state->Anchor.x + 48, state->Anchor.y + 72, 96, 24 }, state->ServerNameInputText, 128, state->ServerNameInputEditMode)) state->ServerNameInputEditMode = !state->ServerNameInputEditMode;
        GuiLabel((Rectangle){ state->Anchor.x + 48, state->Anchor.y + 56, 120, 24 }, "Server Name:");
        if (GuiSpinner((Rectangle){ state->Anchor.x + 224, state->Anchor.y + 72, 96, 24 }, NULL, &state->MaxPlayersInputValue, 0, 100, state->MaxPlayersInputEditMode)) state->MaxPlayersInputEditMode = !state->MaxPlayersInputEditMode;
        GuiLabel((Rectangle){ state->Anchor.x + 224, state->Anchor.y + 56, 120, 24 }, "Max Players:");
        if (GuiTextBox((Rectangle){ state->Anchor.x + 48, state->Anchor.y + 168, 144, 24 }, state->HostNameInputText, 128, state->HostNameInputEditMode)) state->HostNameInputEditMode = !state->HostNameInputEditMode;
        if (GuiTextBox((Rectangle){ state->Anchor.x + 216, state->Anchor.y + 168, 96, 24 }, state->PortInputText, 128, state->PortInputEditMode)) state->PortInputEditMode = !state->PortInputEditMode;
        GuiLabel((Rectangle){ state->Anchor.x + 216, state->Anchor.y + 152, 120, 24 }, "Port");
        GuiLabel((Rectangle){ state->Anchor.x + 48, state->Anchor.y + 152, 120, 24 }, "Hostname/IP:");
        GuiCheckBox((Rectangle){ state->Anchor.x + 24, state->Anchor.y + 240, 24, 24 }, "LAN Game", &state->LanGameChecked);
        GuiLabel((Rectangle){ state->Anchor.x + 48, state->Anchor.y + 192, 264, 16 }, "[NOTE] Not required for LAN Games.");
        state->HostBtnPressed = GuiButton((Rectangle){ state->Anchor.x + 120, state->Anchor.y + 240, 96, 24 }, "Host"); 
        state->JoinBtnPressed = GuiButton((Rectangle){ state->Anchor.x + 240, state->Anchor.y + 240, 96, 24 }, "Join"); 
        if (GuiTextBox((Rectangle){ state->Anchor.x + 152, state->Anchor.y + 72, 64, 24 }, state->TextBox015Text, 128, state->TextBox015EditMode)) state->TextBox015EditMode = !state->TextBox015EditMode;
        GuiLabel((Rectangle){ state->Anchor.x + 152, state->Anchor.y + 56, 120, 24 }, "Port");
    }
}

#endif // GUI_STARTMENU_IMPLEMENTATION
