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
u8* const princess_sps[8] = { g_princess_0, g_princess_1, g_princess_2, g_princess_3, g_princess_4, g_princess_5, g_princess_6, g_princess_7 };
u8* const agent0_sps[8]   = { g_agent0_0, g_agent0_1, g_agent0_2, g_agent0_3, g_agent0_4, g_agent0_5, g_agent0_6, g_agent0_7 };

void initCPC() {
   cpct_disableFirmware();
   cpct_setPalette(g_palette, 16);
   cpct_setVideoMode(0);
   // Copy agent bytes to their places
   cpct_memcpy((void*)princess_sps_add, princess_sps, sizeof(princess_sps));
   cpct_memcpy((void*)  agent0_sps_add,   agent0_sps, sizeof(  agent0_sps));
}

void main(void) {
   // Set up new location for the stack (to use 0x8000-0xBFFF as backbuffer)
   cpct_setStackLocation(0x8000);

   // Initialize
   initCPC();
   GM_initialize();

   // Loop forever
   while (1) {
      GM_update();
      cpct_waitVSYNC();
      GM_draw();
   }
}
