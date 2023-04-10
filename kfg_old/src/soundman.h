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
#ifndef __SOUNDMAN_H__
#define __SOUNDMAN_H__

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC TYPES
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// SFX Types
typedef enum {
//      SFX_shoot    = 0x0c
//   ,  SFX_punch    = 0x0d
//   ,  SFX_getItem  = 0x0f
//   ,  SFX_die      = 0x10
//   ,  SFX_enemyDie = 0x11
      SFX_getItem  = 2
   ,  SFX_beingHit = 4
   ,  SFX_shoot    = 6
   ,  SFX_punch    = 7
   ,  SFX_die      = 9
   ,  SFX_enemyDie = 9
} TSFXType;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void  MM_playSFX(TSFXType sfx);

#endif