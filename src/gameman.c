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
#include "screenman.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

TEntity *hero;
u8 mgm_level;

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
   LM_initialize(0);
   hero = EM_createEntity(10, 95, E_Princess);
   mgm_level = 0;
}

///////////////////////////////////////////////////////////////
/// GM_nextLevel
///   Advance to the next level
///////////////////////////////////////////////////////////////
void GM_nextLevel() {
   ++mgm_level;
   EM_leaveOnlyHero(10, 95);
   LM_initialize(mgm_level);
}

///////////////////////////////////////////////////////////////
/// GM_update
///   Updates the state of the game
///////////////////////////////////////////////////////////////
void GM_update() {
   if (LM_isLevelFinished() || cpct_isKeyPressed(Key_G)) 
      GM_nextLevel();
//   cpct_setBorder(HW_MAGENTA);
   EM_update();
   LM_update(hero->pos[2].x);
   EM_clear();
//   cpct_setBorder(HW_BLACK);
   cpct_waitVSYNC();
}

///////////////////////////////////////////////////////////////
/// GM_draw
///   Draws next frame of the game under execution
///////////////////////////////////////////////////////////////
void GM_draw() {
   LM_draw();
   EM_draw();
//   cpct_setBorder(HW_GREEN);
   cpct_waitVSYNC();
   SM_switchScreenBuffers();
}
