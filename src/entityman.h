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
   , A_HitAttack = 0x10
} TEntityActions;

// Entity Identifiers
typedef enum {
     E_Princess  = 0x00
   , E_Agent     = 0x01
   , E_HitBow    = 0x02
} TEntityIDs;

// Entity Types
typedef enum {
     T_Destroyed = 0x00
   , T_Princess  = 0x01
   , T_Agent     = 0x02
   , T_HitBow    = 0x04
} TEntityType;

// Side towards an entity is facing
typedef enum {
      F_Right    = 0x00
   ,  F_Left     = 0x01
} TFacing;

// Point
typedef struct {
   u8 x, y;
} TPoint;

// Entity definition
typedef struct Entity {
   TPoint         pos[3];  // 3 Locations
   u8               w, h;  // Width-height
   u8              drawt;  // Times the entity has still to be drawn
   TEntityType      type;  // Type of entity
   u8             status;  // Status byte for the entity (0: facing 0 right, 1 left)
   u8**        spriteset;  // Set of sprites
   u8                  t;  // Time in this state
   i8             energy;  // Energy this entity carries (may be life or hit strength)
   u8*            sprite;  // Sprite currently being drawn
   u8         nextAction;  // Next action to be performed
   void (*fstate)(struct Entity *e);    // Entity state function 
} TEntity;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
TEntity* EM_createEntity(u8 x, u8 y, u8 entType);
TEntity* EM_getHero();
u8       EM_getNumEnemies();
void     EM_update();
void     EM_draw();
void     EM_clear();
void     EM_move(TEntity *e);
void     EM_moveEntityX(TEntity* e, i8 pixels);
void     EM_scroll(i8 pixels);
void     EM_addEntity2Draw(TEntity *e2d);
void     EM_deleteEntity(TEntity *e);
void     EM_nextWalkingFrame(TEntity* e);

#endif