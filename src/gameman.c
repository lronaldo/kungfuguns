//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Kung Fu Guns: A CPCtelera game made for #bitbitjam3
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
#include "scoreman.h"
#include "utils.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

TEntity *hero;
u8  mgm_level;
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
   EM_initialize();
   CM_inititalize(24, 0);
   hero = EM_createEntity(10, 95, E_Princess);
   hero->energy = 24;
   mgm_level = 0;
}

///////////////////////////////////////////////////////////////
/// GM_startmenu
///   Initializes the menu
///////////////////////////////////////////////////////////////
void GM_startmenu () {
   // Compile-time constants
   enum {
      syncs       = 4,
      secs        = 15,
      intspersec  = 300,
      num_loops   = (secs * intspersec) / syncs
   };

   LM_initialize(getRand() & 1);
   LM_draw();

   while (1) {
      u16 c = num_loops;
      TWaitStatuses st = WS_Wait;

      LM_setOffset(getRand());
      LM_drawBg(SM_scrBuf());

      cpct_setDrawCharM0( 15, 1 );
      cpct_drawStringM0 ("KUNG FU GUNS"
                        ,  cpctm_screenPtr(CPCT_VMEM_START, 13, 70));

      while (--c) {
         cpct_setDrawCharM0( getRand() & 0x07 + 2, 10);
         cpct_drawStringM0 ("PRESS ANY KEY"
                           ,  cpctm_screenPtr(CPCT_VMEM_START, 12, 140));
         st = wait4newKeyPressed(st);
         if ( st == WS_NewKeyPressed )
            return;
         waitNVSyncs(syncs);
      }
   }
}

///////////////////////////////////////////////////////////////
/// GM_gameOver
///   Initializes the menu
///////////////////////////////////////////////////////////////
void GM_gameOver() {
   // Compile-time constants
   enum {
      syncs       = 3,
      secs        = 10,
      intspersec  = 300,
      num_loops   = (secs * intspersec) / syncs
   };

   u16 c = num_loops;
   TWaitStatuses st = WS_Wait;

   while (--c) {
      cpct_setDrawCharM0( c & 0x0F, 1);
      cpct_drawStringM0 ("GAME OVER"
                        ,  cpctm_screenPtr(CPCT_VMEM_START, 20, 70));
      st = wait4newKeyPressed(st);
      if (st == WS_NewKeyPressed)
         return;
      waitNVSyncs(syncs);
   }
}

///////////////////////////////////////////////////////////////
/// GM_nextLevel
///   Advance to the next level
///////////////////////////////////////////////////////////////
void GM_nextLevel() {
   mgm_level = (mgm_level + 1) & 1; 
   EM_leaveOnlyHero(10, 95);
   LM_initialize(mgm_level);
}

///////////////////////////////////////////////////////////////
/// GM_update
///   Updates the state of the game
///////////////////////////////////////////////////////////////
u8 GM_update() {
   if (LM_isLevelFinished() || cpct_isKeyPressed(Key_G)) 
      GM_nextLevel();
//   cpct_setBorder(HW_MAGENTA);
   EM_update();
   LM_update(hero->pos[2].x);
   EM_clear();
   CM_draw();
//   cpct_setBorder(HW_BLACK);
   cpct_waitVSYNC();

   return CM_getLife();
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
