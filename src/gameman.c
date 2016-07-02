//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CodePrincess: A CPCtelera game made for #bitbitjam3
//  Copyright (C) 2016 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <cpctelera.h>
#include "gameman.h"
#include "entityman.h"
#include "levelman.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

TEntity *hero;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
/// GM_initialize
///   Initializes the game manager object
///////////////////////////////////////////////////////////////
void GM_initialize() {
   // Initialize entity manager and create hero
   EM_initialize();
   LM_initialize();
   hero = EM_createEntity(10, 95, E_Princess);
}

///////////////////////////////////////////////////////////////
/// GM_getUserInput
///   Gets user input and transforms it into actions
///////////////////////////////////////////////////////////////
void GM_getUserInput() {
   u8 nextAction = 0;
   
   // Read keyboard and interpret action requested
//   cpct_scanKeyboard_if();
   if (cpct_isKeyPressed(Key_CursorLeft)) {
      nextAction |= A_MoveLeft;
   } else if (cpct_isKeyPressed(Key_CursorRight)) {
      nextAction |= A_MoveRight;
   }
   if (cpct_isKeyPressed(Key_CursorUp)) {
      nextAction |= A_MoveUp;
   } else if (cpct_isKeyPressed(Key_CursorDown)) {
      nextAction |= A_MoveDown;
   }

   // Set next action for hero
   hero->nextAction = nextAction;
}

///////////////////////////////////////////////////////////////
/// GM_update
///   Updates the state of the game
///////////////////////////////////////////////////////////////
void GM_update() {
   GM_getUserInput();
   EM_update();
   LM_update(hero->x);
}

///////////////////////////////////////////////////////////////
/// GM_draw
///   Draws next frame of the game under execution
///////////////////////////////////////////////////////////////
void GM_draw() {
   cpct_setBorder(HW_BLUE);
   LM_draw();
   cpct_setBorder(HW_RED);
   EM_clear();
   cpct_setBorder(HW_GREEN);
   EM_draw();
   cpct_setBorder(HW_BLACK);
   EM_clearDrawEntityBuffer();
}
