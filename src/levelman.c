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
#include "entityman.h"
#include "levels/level0.h"
#include "tiles/tileset0.h"
#include "tiles/tileset1.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Background splitted into 2 parts
u8 g_bg[2][20*17];

// Tilesets
u8* const g_tileset0[16] = {g_tileset0_00, g_tileset0_01, g_tileset0_02, g_tileset0_03, g_tileset0_04, g_tileset0_05, g_tileset0_06, g_tileset0_07, g_tileset0_08, g_tileset0_09, g_tileset0_10, g_tileset0_11, g_tileset0_12, g_tileset0_13, g_tileset0_14, g_tileset0_15};
u8* const g_tileset1[16] = {g_tileset1_00, g_tileset1_01, g_tileset1_02, g_tileset1_03, g_tileset1_04, g_tileset1_05, g_tileset1_06, g_tileset1_07, g_tileset1_08, g_tileset1_09, g_tileset1_10, g_tileset1_11, g_tileset1_12, g_tileset1_13, g_tileset1_14, g_tileset1_15};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/// fillBG
///   Fills in the background with some level tiles
///////////////////////////////////////////////////////////////
void fillBg(u8* lev, u8* bg, u16 idx) {
   u8 i, j;
   for(i=0; i < 17;i++) {
      for(j=0; j < 20;j++) {
         *bg = cpct_get4Bits(lev, idx);
         bg++;
         idx++;
      }
      idx += 320 - 20;
   }
}

///////////////////////////////////////////////////////////////
/// drawBg
///   Draws the background completely
///////////////////////////////////////////////////////////////
#define SCR_P1 cpctm_screenPtr(CPCT_VMEM_START,  0, 40)
#define SCR_P2 cpctm_screenPtr(CPCT_VMEM_START, 40, 40)
void drawBg() {
   cpct_etm_setTileset2x4(g_tileset0);
   cpct_etm_drawTilemap2x4 (20, 17, SCR_P1, g_bg[0]);
   cpct_etm_setTileset2x4(g_tileset1);
   cpct_etm_drawTilemap2x4 (20, 17, SCR_P2, g_bg[1]);
}

///////////////////////////////////////////////////////////////
/// LM_redrawBackgroundBox
///   Redraws part of the background as a defined box
///////////////////////////////////////////////////////////////
void LM_redrawBackgroundBox(u8 x, u8 y, u8 w, u8 h) {
   u8 h_up, h_down, h_start;
   // Check if I'm ocluding the wall
   if (y < 108) {
      // General calculations
      u8 tx1, tx2, tw1 = 0, tw2 = 0;
      u8 ty   = (y - 40) >> 2;
      u8 tw   = (w >> 1) + (x & 1);
      h_up    = 108 - y;
      h_down  = h - h_up;
      h_start = 108;
      h_up    = (h_up >> 2) + (h_up & 3 ? 1 : 0);

      // Check which walls I am ocluding
      if (x < 40 - w) {
         tx1 = x >> 1;
         tw1 = tw;
      } else if (x >= 40) {
         tx2 = (x - 40) >> 1;
         tw2 = tw;
      } else {
         tx1 = x >> 1;
         tx2 = 0;
         tw1 = 20 - tx1;
         tw2 = tw - tw1;
      }

      // Redraw walls
      if (tw1) {
         cpct_etm_setTileset2x4(g_tileset0);
         cpct_etm_drawTileBox2x4(tx1, ty, tw1, h_up, 20, SCR_P1, g_bg[0]);
      }
      if (tw2) {
         cpct_etm_setTileset2x4(g_tileset1);
         cpct_etm_drawTileBox2x4(tx2, ty, tw2, h_up, 20, SCR_P2, g_bg[1]);
      }
   } else {
      // Not ocluding the wall, only terrain
      h_down  = h;
      h_start = y;
   }

   // Terrain part
   if (h_down) {
      u8* scr = cpct_getScreenPtr(CPCT_VMEM_START, x, h_start);
      cpct_drawSolidBox(scr, 0x33, w, h_down);
   }
}

///////////////////////////////////////////////////////////////
/// LM_initialize
///   Initializes the level manager object
///////////////////////////////////////////////////////////////
void LM_initialize() {
   // TODO: Redesign
   fillBg(g_level0, g_bg[0],  0);
   fillBg(g_level0, g_bg[1], 20);
}

///////////////////////////////////////////////////////////////
/// LM_update
///   Updates the state of the level
///////////////////////////////////////////////////////////////
void LM_update() {
}

///////////////////////////////////////////////////////////////
/// LM_draw
///   Draws the level and redraws background when required
///////////////////////////////////////////////////////////////
#define SCR_FL1 cpctm_screenPtr(CPCT_VMEM_START,  0, 108)
#define SCR_FL2 cpctm_screenPtr(CPCT_VMEM_START, 40, 108)
void LM_draw() {
   // TODO: Redesign
   drawBg();
   cpct_drawSolidBox(SCR_FL1, 0x33, 40, 92);
   cpct_drawSolidBox(SCR_FL2, 0x33, 40, 92);
}
