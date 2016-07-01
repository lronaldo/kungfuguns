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
#include "sprites/princess.h"
#include "sprites/agent0.h"
#include "sprites/spritesets.h"

// TODO: This should be placed over hardware backbuffer as initialization code
u8* const princess_sps[12] = { g_princess_00, g_princess_01, g_princess_02, g_princess_03, g_princess_04, g_princess_05, g_princess_06, g_princess_07, g_princess_08, g_princess_09, g_princess_10, g_princess_11 };
u8* const agent0_sps[6]    = { g_agent0_0, g_agent0_1, g_agent0_2, g_agent0_3, g_agent0_4, g_agent0_5 };
void initialize() {
   cpct_disableFirmware();
   cpct_setPalette(g_palette, 16);
   cpct_setVideoMode(0);
   // Copy agent bytes to their places
   cpct_memcpy((void*)princess_sps_add, princess_sps, sizeof(princess_sps));
   cpct_memcpy((void*)  agent0_sps_add,   agent0_sps, sizeof(  agent0_sps));
}

void main(void) {
   initialize();

   GM_initialize();

   // Loop forever
   while (1) {
      GM_update();
      cpct_waitVSYNC();
      GM_draw();
   }
}
