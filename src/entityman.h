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

#ifndef __ENTITYMAN_H__
#define __ENTITYMAN_H__

#include <types.h>

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DEFINED PUBLIC TYPES
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Entity Actions
typedef enum {
     A_MoveLeft  = 0x01
   , A_MoveRight = 0x02
   , A_MoveUp    = 0x04
   , A_MoveDown  = 0x08
} TEntityActions;

// Entity Types
typedef enum {
     E_Princess  = 0x00
   , E_Agent     = 0x01
} TEntityTypes;

// Entity Statuses
typedef enum {
     // Main Character
     S_HeroWait  = 0x00

     // Enemies
   , S_AgentWait = 0x80
   , S_AgentWalk = 0x81
} TEntityStatus;

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
   u8  energy;    // Energy this entity carries (may be life or hit strength)
} TEntity;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void     EM_initialize();
TEntity* EM_createEntity(u8 x, u8 y, u8 entType);
void     EM_update();
void     EM_draw();
void     EM_clear();
void     EM_scroll(i8 pixels);
void     EM_moveEntityX(TEntity* e, i8 pixels);
void     EM_clearDrawEntityBuffer();

#endif