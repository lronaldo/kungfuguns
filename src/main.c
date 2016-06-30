//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2015 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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
#include "sprites/princess.h"
#include "levels/level0.h"
#include "tiles/tileset0.h"
#include "tiles/tileset1.h"

// Print transparency mask table
cpctm_createTransparentMaskTable(g_alphatable, 0x100, M0, 0);

// Sprite Sets
u8* const princess_sps[12] = {g_princess_00, g_princess_01, g_princess_02, g_princess_03, g_princess_04, g_princess_05, g_princess_06, g_princess_07, g_princess_08, g_princess_09, g_princess_10, g_princess_11};

// Entity Actions
typedef enum {
     A_MoveLeft  = 0x01
   , A_MoveRight = 0x02
   , A_MoveUp    = 0x04
   , A_MoveDown  = 0x08
} TEntityActions;

// Entity definition
typedef struct {
   u8  x, y;      // Location
   u8 ox, oy;     // Previous location
   u8  w, h;      // Width-height
   u8** spriteset;// Set of sprites
   u8* sprite;    // Concrete sprite
   u8  status;    // Entity status
   u8  nextAction;// Next action to be performed
   u8  t;         // Time in this status
} TEntity;

// Entities
#define MAX_ENTITIES 8
TEntity  g_entities[MAX_ENTITIES];
TEntity *g_entities2Draw[MAX_ENTITIES];
u8 g_numEntities;
u8 g_nEnt2Draw;

// Tilesets
u8* const g_tileset0[16] = {g_tileset0_00, g_tileset0_01, g_tileset0_02, g_tileset0_03, g_tileset0_04, g_tileset0_05, g_tileset0_06, g_tileset0_07, g_tileset0_08, g_tileset0_09, g_tileset0_10, g_tileset0_11, g_tileset0_12, g_tileset0_13, g_tileset0_14, g_tileset0_15};
u8* const g_tileset1[16] = {g_tileset1_00, g_tileset1_01, g_tileset1_02, g_tileset1_03, g_tileset1_04, g_tileset1_05, g_tileset1_06, g_tileset1_07, g_tileset1_08, g_tileset1_09, g_tileset1_10, g_tileset1_11, g_tileset1_12, g_tileset1_13, g_tileset1_14, g_tileset1_15};

// Background splitted into 2 parts
u8 g_bg[2][20*17];

void initialize() {
   TEntity* e = g_entities;
   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setPalette(g_palette, 16);
   g_numEntities = 1;
   g_nEnt2Draw = 0;
   e->x = e->ox = 10;
   e->y = e->oy = 95;
   e->w = 8;
   e->h = 27;
   e->spriteset = princess_sps;
   e->sprite = princess_sps[0];
   e->status = 0;
   e->t = 0;
} 

void drawEntity(TEntity *e) {
   u8 *scr;
   scr = cpct_getScreenPtr(CPCT_VMEM_START, e->x, e->y);
   cpct_drawSpriteMaskedAlignedTable(e->sprite, scr, e->w, e->h, g_alphatable);
}

#define SCR_P1 cpctm_screenPtr(CPCT_VMEM_START,  0, 40)
#define SCR_P2 cpctm_screenPtr(CPCT_VMEM_START, 40, 40)
void clearEntity(TEntity *e) {
   u8 h_up, h_down, h_start;
   // Check if I'm ocluding the wall
   if (e->oy < 108) {
      // General calculations
      u8 tx1, tx2, tw1 = 0, tw2 = 0;
      u8 ty = (e->oy - 40) >> 2;
      u8 tw = (e->w >> 1) + (e->ox & 1);
      h_up = 108 - e->oy;
      h_down = e->h - h_up;
      h_start = 108;
      h_up = (h_up >> 2) + (h_up & 3 ? 1 : 0);

      // Check which walls I am ocluding
      if (e->ox < 40 - e->w) {
         tx1 = e->ox >> 1;
         tw1 = tw;
      } else if (e->ox >= 40) {
         tx2 = (e->ox - 40) >> 1;
         tw2 = tw;
      } else {
         tx1 = e->ox >> 1;
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
      h_up = 0;
      h_down = e->h;
      h_start = e->oy;
   }

   if (h_down) {
      u8* scr = cpct_getScreenPtr(CPCT_VMEM_START, e->ox, h_start);
      cpct_drawSolidBox(scr, 0x33, e->w, h_down);
   }
}

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

void drawBg() {
   cpct_etm_setTileset2x4(g_tileset0);
   cpct_etm_drawTilemap2x4 (20, 17, SCR_P1, g_bg[0]);
   cpct_etm_setTileset2x4(g_tileset1);
   cpct_etm_drawTilemap2x4 (20, 17, SCR_P2, g_bg[1]);
}

void addEntity2Draw(TEntity *e) {
   g_entities2Draw[g_nEnt2Draw] = e;
   g_nEnt2Draw++;
}

void drawEntities() {
   u8 i;
   TEntity **e = g_entities2Draw;

   for(i=0; i < g_nEnt2Draw; i++) {
      drawEntity(*e);
      e++;
   }
}

void clearEntities() {
   u8 i;
   TEntity **e = g_entities2Draw;

   for(i=0; i < g_nEnt2Draw; i++) {
      clearEntity(*e);
      e++;
   }
}

void updateEntities() {
   TEntity *e = g_entities;
   u8 i;

   for(i=0; i < g_numEntities; i++) {
      u8 a = e->nextAction;
      e->ox = e->x;
      e->oy = e->y;
      // Possible actions
      if (a & A_MoveLeft  && e->x > 0) {
         e->x--; 
      } else if (a & A_MoveRight && e->x < 80 - e->w) {
         e->x++; 
      } 
      if (a & A_MoveUp    && e->y > 108 - e->h) {
         e->y--; 
      } else if (a & A_MoveDown  && e->y < 200 - e->h) {
         e->y++; 
      }
      if (a) 
         addEntity2Draw(e);
      e++;
   }
}

void userInput() {
   cpct_scanKeyboard_f();

   g_entities[0].nextAction = 0;
   if (cpct_isKeyPressed(Key_CursorLeft)) {
      g_entities[0].nextAction |= 1;
   } else if (cpct_isKeyPressed(Key_CursorRight)) {
      g_entities[0].nextAction |= 2;
   }
   if (cpct_isKeyPressed(Key_CursorUp)) {
      g_entities[0].nextAction |= 4;
   } else if (cpct_isKeyPressed(Key_CursorDown)) {
      g_entities[0].nextAction |= 8;
   }
}

#define SCR_PR  cpctm_screenPtr(CPCT_VMEM_START, 25,  95)
#define SCR_FL1 cpctm_screenPtr(CPCT_VMEM_START,  0, 108)
#define SCR_FL2 cpctm_screenPtr(CPCT_VMEM_START, 40, 108)
void main(void) {
   initialize();

   fillBg(g_level0, g_bg[0],  0);
   fillBg(g_level0, g_bg[1], 20);
   drawBg();
   cpct_drawSolidBox(SCR_FL1, 0x33, 40, 92);
   cpct_drawSolidBox(SCR_FL2, 0x33, 40, 92);
   drawEntity(g_entities + 0);

   // Loop forever
   while (1) {
      userInput();
      updateEntities();
      cpct_waitVSYNC();
      cpct_setBorder(HW_RED);
      clearEntities();
      cpct_setBorder(HW_GREEN);
      drawEntities();
      cpct_setBorder(HW_BLACK);
      g_nEnt2Draw = 0;
   }
}
