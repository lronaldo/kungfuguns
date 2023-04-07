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
#include "entityman.h"
#include "levels/level0.h"
#include "tiles/tileset0.h"
#include "tiles/tileset1.h"
#include "tiles/tileset2.h"
#include "tiles/tileset3.h"

#define ENEMIES_LOCK_SCROLL      3
#define SWITCH_PTR(P1, P2) { void* auxp; auxp=(void*)(P1); (P1)=(P2); (P2)=auxp; }
#define NLEVELS                  2
#define LEVEL_CHUCKS_SIZE       16

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// INTERNAL STRUCTURES
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

typedef enum {
     LS_RedrawBg    = 0x80   // Flag signaling background redraw
   , LS_RedrawFloor = 0x40   // Flag signaling floor redraw
   , LS_ScrollLock  = 0x04   // Flag for locking scroll
} TLevelStatus;

typedef struct {
   u8*       map;
   u8 floorcolor;
   u8** tilesets[LEVEL_CHUCKS_SIZE];
} TLevelData;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

u8   m_backgroundMaps[2][20*17]; // 2 Tilemaps for present background
u8*  m_bgMapPtr[2];              // Pointers to the 2 background maps for switching
u8** m_bgTilPtr[2];              // Pointers to the 2 background tilesets 
u8   m_levelStatus;              // Flags controlling level status (76543210 > 7-6: redraw Floor, Bg, 1-0: Level number, 2: scroll_lock)
u16  m_levelOffset;              // Offset of the hero into the level (for scrolling background)

// Level data
TLevelData *m_level;             // Pointer to the current level

// Tilesets
extern u8* const g_tileset0[16];
extern u8* const g_tileset1[16];
extern u8* const g_tileset2[16];
extern u8* const g_tileset3[16];

// Level data
extern TLevelData m_levels[NLEVELS];

///////////////////////////////////////////////////////////////
/// LM_dummy_init
///   Dummy function for static data initializing
///////////////////////////////////////////////////////////////
void LM_dummy_init() __naked {
   __asm
   ///-------------------------------
   /// TILESETS
   ///-------------------------------

   // Tileset 0
   _g_tileset0::
   .dw _g_tileset0_00, _g_tileset0_01, _g_tileset0_02, _g_tileset0_03
   .dw _g_tileset0_04, _g_tileset0_05, _g_tileset0_06, _g_tileset0_07
   .dw _g_tileset0_08, _g_tileset0_09, _g_tileset0_10, _g_tileset0_11
   .dw _g_tileset0_12, _g_tileset0_13, _g_tileset0_14, _g_tileset0_15

   // Tileset 1
   _g_tileset1::
   .dw _g_tileset1_00, _g_tileset1_01, _g_tileset1_02, _g_tileset1_03
   .dw _g_tileset1_04, _g_tileset1_05, _g_tileset1_06, _g_tileset1_07
   .dw _g_tileset1_08, _g_tileset1_09, _g_tileset1_10, _g_tileset1_11
   .dw _g_tileset1_12, _g_tileset1_13, _g_tileset1_14, _g_tileset1_15

   _g_tileset2::
   .dw _g_tileset2_00, _g_tileset2_01, _g_tileset2_02, _g_tileset2_03
   .dw _g_tileset2_04, _g_tileset2_05, _g_tileset2_06, _g_tileset2_07
   .dw _g_tileset2_08, _g_tileset2_09, _g_tileset2_10, _g_tileset2_11
   .dw _g_tileset2_12, _g_tileset2_13, _g_tileset2_14, _g_tileset2_15

   _g_tileset3::
   .dw _g_tileset3_00, _g_tileset3_01, _g_tileset3_02, _g_tileset3_03
   .dw _g_tileset3_04, _g_tileset3_05, _g_tileset3_06, _g_tileset3_07
   .dw _g_tileset3_08, _g_tileset3_09, _g_tileset3_10, _g_tileset3_11
   .dw _g_tileset3_12, _g_tileset3_13, _g_tileset3_14, _g_tileset3_15

   ///-------------------------------
   /// LEVELS
   ///-------------------------------
   _m_levels::
   // LEVEL 0
      .dw   _g_level0    // map / tilesets
      .db   0x0F         // floorcolor
      .dw   _g_tileset0, _g_tileset0, _g_tileset0, _g_tileset0 
      .dw   _g_tileset0, _g_tileset0, _g_tileset0, _g_tileset0
      .dw   _g_tileset0, _g_tileset0, _g_tileset0, _g_tileset0
      .dw   _g_tileset0, _g_tileset0, _g_tileset1, _g_tileset1
   // LEVEL 1
      .dw   _g_level1    // map / tilesets
      .db   0x33         // floorcolor
      .dw   _g_tileset3, _g_tileset2, _g_tileset3, _g_tileset2
      .dw   _g_tileset3, _g_tileset2, _g_tileset3, _g_tileset2
      .dw   _g_tileset3, _g_tileset2, _g_tileset3, _g_tileset2
      .dw   _g_tileset3, _g_tileset2, _g_tileset3, _g_tileset2
   __endasm;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/// fillBG
///   Fills in the background with some level tiles
///////////////////////////////////////////////////////////////
void fillBg(u8* bg, u16 idx) {
   u8 i, j;
   u8* map = m_level->map;

   for(i=0; i < 17;i++) {
      for(j=0; j < 20;j++) {
         *bg = cpct_get4Bits(map, idx);
         bg++; idx++;
      }
      idx += 320 - 20;
   }
}

///////////////////////////////////////////////////////////////
/// drawBg
///   Draws the background completely
///////////////////////////////////////////////////////////////
void LM_drawBg(u8 *buf) {
   u8    *scr = buf + ((u8*)cpctm_screenPtr(CPCT_VMEM_START, 40, 40) - CPCT_VMEM_START);
   u8       i = 2;

   while (i) {
      --i;
      cpct_etm_setTileset2x4  (m_bgTilPtr[i]);
      cpct_etm_drawTilemap2x4 (20, 17, scr, m_bgMapPtr[i]);
      scr = scr - 40;
   }
}

///////////////////////////////////////////////////////////////
/// LM_redrawBackgroundBox
///   Redraws part of the background as a defined box
///////////////////////////////////////////////////////////////
void LM_redrawBackgroundBox(u8 x, u8 y, u8 w, u8 h, u8* buf) {
   u8 h_up, h_down, h_start;
   // Check if I'm ocluding the wall
   if (y < 108) {
      // General calculations
      u8 tx1 = 0, tx2 = 0, tw1 = 0, tw2 = 0;
      u8 ty   = (y - 40) >> 2;
      u8 tw   = (w >> 1) + (x & 1);
      h_up    = 108 - y;
      h_down  = (h_up > h) ? 0 : h - h_up;
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
         cpct_etm_setTileset2x4 (m_bgTilPtr[0]);
         cpct_etm_drawTileBox2x4(tx1, ty, tw1, h_up, 20
                                 , buf + ((u8*)cpctm_screenPtr(CPCT_VMEM_START, 0, 40) - CPCT_VMEM_START)
                                 , m_bgMapPtr[0]);
      }
      if (tw2) {
         cpct_etm_setTileset2x4 (m_bgTilPtr[1]);
         cpct_etm_drawTileBox2x4(tx2, ty, tw2, h_up, 20
                                 , buf + ((u8*)cpctm_screenPtr(CPCT_VMEM_START, 40, 40) - CPCT_VMEM_START)
                                 , m_bgMapPtr[1]);

      }
   } else {
      // Not ocluding the wall, only terrain
      h_down  = h;
      h_start = y;
   }

   // Terrain part
   if (h_down) {
      u8* scr = cpct_getScreenPtr(buf, x, h_start);
      cpct_drawSolidBox(scr, m_level->floorcolor, w, h_down);
   }
}


///////////////////////////////////////////////////////////////
/// LM_setOffset
///   Sets the offset of the scroll
///////////////////////////////////////////////////////////////
void LM_setOffset(u8 off) {
   m_levelOffset = off & 0x0F;
   if (m_levelOffset) m_levelOffset--;
   fillBg(m_bgMapPtr[0], 20 * m_levelOffset);
   fillBg(m_bgMapPtr[1], 20 * (m_levelOffset + 1));
}

///////////////////////////////////////////////////////////////
/// LM_initialize
///   Initializes the level manager object
///////////////////////////////////////////////////////////////
void LM_initialize(u8 level) {
   // At first, redraw floor and Background is required
   // And select level 0 (2 LSbits = 0)
   m_levelStatus  = LS_RedrawFloor | LS_RedrawBg;
   
   // Set level, offset and background pointers
   m_level        = m_levels + level;
   m_levelOffset  = 0;
   m_bgMapPtr[0]  = m_backgroundMaps[0];
   m_bgMapPtr[1]  = m_backgroundMaps[1];
   m_bgTilPtr[0]  = m_level->tilesets[0];
   m_bgTilPtr[1]  = m_level->tilesets[1];

   // Fill in present background part of the level map
   fillBg(m_bgMapPtr[0], 0);
   fillBg(m_bgMapPtr[1], 20);
}


///////////////////////////////////////////////////////////////
/// LM_isLevelFinished
///   Checks if the level is finished
///////////////////////////////////////////////////////////////
u8 LM_isLevelFinished() {
   return   ( m_levelOffset == LEVEL_CHUCKS_SIZE - 2 )
         && ( EM_getNumEnemies() == 0 );
}

///////////////////////////////////////////////////////////////
/// LM_scrollRight
///   Scrolls map 1 place to the right
///////////////////////////////////////////////////////////////
void LM_scrollRight() {
   // Move right part of the map to the left switching pointers
   // Then offset the map and fill the right part with new background
   m_levelOffset++;
   SWITCH_PTR(m_bgMapPtr[0], m_bgMapPtr[1]);
   m_bgTilPtr[0] = m_bgTilPtr[1];
   m_bgTilPtr[1] = m_level->tilesets[m_levelOffset+1];
   fillBg(m_bgMapPtr[1], 20*(m_levelOffset+1));

   // Mark for redrawing
   m_levelStatus |= LS_RedrawBg;

   // Scroll all entities 40 points left (80 pixels) left
   EM_scroll(-40);

   // TEST
   EM_createEntity(60, 80, E_Agent);
}

///////////////////////////////////////////////////////////////
/// LM_update
///   Updates the state of the level
///////////////////////////////////////////////////////////////
void LM_update(u8 hero_x) {
   u8 nEnem = EM_getNumEnemies();

   // Check for scroll
   if (m_levelStatus & LS_ScrollLock) {
      if (!nEnem)
         m_levelStatus &= ~LS_ScrollLock;
   } else {
      if (hero_x > 68 && m_levelOffset < LEVEL_CHUCKS_SIZE - 2) {
         LM_scrollRight();
      } else if (nEnem >= ENEMIES_LOCK_SCROLL) {
         m_levelStatus |= LS_ScrollLock;
      }
   }
}

///////////////////////////////////////////////////////////////
/// LM_draw
///   Draws the level and redraws background when required
///////////////////////////////////////////////////////////////
u8* const memplaces[4] = { 
      cpctm_screenPtr(0x8000,  0, 108)
   ,  cpctm_screenPtr(0x8000, 40, 108)
   ,  cpctm_screenPtr(0xC000,  0, 108)
   ,  cpctm_screenPtr(0xC000, 40, 108)
};
void LM_draw() {
   // Redraw floor when required
   if (m_levelStatus & LS_RedrawFloor) {
      u8     i = 4;
      u8** scr = (void*)memplaces;
      while (i) {
         cpct_drawSolidBox(*scr, m_level->floorcolor, 40, 92);
         --i; ++scr;
      }
      m_levelStatus ^= LS_RedrawFloor;
   }
   
   // Redraw Background when required
   if (m_levelStatus & LS_RedrawBg) {
      LM_drawBg((void*)0x8000);
      LM_drawBg(CPCT_VMEM_START);
      m_levelStatus ^= LS_RedrawBg;
   }
}
