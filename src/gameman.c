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
u8  mgm_level;
u8  m_menubg;
extern u32 m_rndState;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void GM_dummy_init() __naked {
   __asm
   _m_rndState:: 
      .dw 0x8a63, 0x7bf1
   __endasm;
}

///////////////////////////////////////////////////////////////
/// getRand
///   Returns a random number from 0 to 255
///////////////////////////////////////////////////////////////
u8 getRand() {
   m_rndState = cpct_nextRandom_mxor_u8 (m_rndState);
   return m_rndState;
}

///////////////////////////////////////////////////////////////
/// GM_startgame
///   Initializes a new game
///////////////////////////////////////////////////////////////
void GM_startgame() {
   // Initialize entity manager and create hero
   LM_initialize(0);
   hero = EM_createEntity(10, 95, E_Princess);
   mgm_level = 0;
}

///////////////////////////////////////////////////////////////
/// GM_startmenu
///   Initializes the menu
///////////////////////////////////////////////////////////////
void GM_startmenu () {
   LM_initialize(getRand() & 1);
   LM_draw();

   while (1) {
      u16 c = 1000;  // 20 secs

      LM_setOffset(getRand());
      LM_drawBg(SM_scrBuf());

      cpct_drawStringM0 ("CODE PRINCESS"
                        ,  cpctm_screenPtr(CPCT_VMEM_START, 14, 70)
                        ,  getRand() & 0x07 + 2, 1);

      cpct_drawStringM0 ("ANY KEY TO START"
                        ,  cpctm_screenPtr(CPCT_VMEM_START, 10, 140)
                        ,  getRand() & 0x07 + 2, 10);

      while (--c) {
         if (cpct_isAnyKeyPressed())
            return;
         cpct_waitVSYNC();
      }
   }
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
