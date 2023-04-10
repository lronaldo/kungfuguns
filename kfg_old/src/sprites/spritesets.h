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

#ifndef __SPRITESETS_H__
#define __SPRITESETS_H__

// Defines with absolute addresses
#define princess_sps_add      0xC7D0
#define princess_sps_add_lo   0xD0
#define princess_sps_add_hi   0xC7
#define agent0_sps_add        0xC7E0
#define agent0_sps_add_lo     0xE0
#define agent0_sps_add_hi     0xC7
#define heroAttack_sps_add    0xC7F0
#define heroAttack_sps_add_lo 0xF0
#define heroAttack_sps_add_hi 0xC7

// Sprite set identifiers (1st byte contains facing flags)
extern u8* const princess_sps[8];
extern u8* const agent0_sps[8];
extern u8* const heroAttack_sps[1];

#endif