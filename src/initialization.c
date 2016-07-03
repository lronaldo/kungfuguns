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
#include "sprites/hit.h"
#include "sprites/agent0.h"
#include "sprites/princess.h"
#include "sprites/spritesets.h"
#include "music/fromscratch.h"

// Locate all this initialization code in the hardware backbuffer area
CPCT_ABSOLUTE_LOCATION_AREA (0x8000);

// Sprite Sets (1st byte contains facing flags)
u8* const princess_sps[9]   = { (u8*)0x00, g_princess_0, g_princess_1, g_princess_2, g_princess_3, g_princess_4, g_princess_5, g_princess_6, g_princess_7 };
u8* const agent0_sps[9]     = { (u8*)0x00, g_agent0_0, g_agent0_1, g_agent0_2, g_agent0_3, g_agent0_4, g_agent0_5, g_agent0_6, g_agent0_7 };
u8* const heroAttack_sps[2] = { (u8*)0x00, g_hit };

void interruptHandler();

void initCPC() {
   // Set the interrupt handler just after first VSYNC
   cpct_waitVSYNC();
   cpct_setInterruptHandler ( interruptHandler );

   // Set Palette and videomode
   cpct_setPalette(g_palette, 16);
   cpct_setVideoMode(0);

   // Initialize Music
   cpct_akp_musicInit(g_renegremix);
   
   // Copy sprite sets and other to spare video memory
   cpct_memcpy((void*)  princess_sps_add,   princess_sps, sizeof(  princess_sps));
   cpct_memcpy((void*)    agent0_sps_add,     agent0_sps, sizeof(    agent0_sps));
   cpct_memcpy((void*)heroAttack_sps_add, heroAttack_sps, sizeof(heroAttack_sps));
}


CPCT_RELOCATABLE_AREA ();