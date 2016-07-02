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
#include "levelman.h"
#include "sprites/princess.h"
#include "sprites/agent0.h"
#include "sprites/spritesets.h"

// Print transparency mask table
cpctm_createTransparentMaskTable(g_alphatable, 0x100, M0, 0);

// Entity types
const u8 k_entityTypes[2][9] = {
   // 0: Princess
   { 
        0x08                  // Size 16x27 px, 8x27 bytes 
      , 0x1B              
      , princess_sps_add_lo   // Pointer to princess_sps split into its 2 bytes
      , princess_sps_add_hi
      , princess_sps_add_lo   // Again to point to the first element (same pointer)
      , princess_sps_add_hi
      , S_Active              // Status Active = 1
      , 0x00                  // t=0
      , 0x64                  // Energy = 100
   }
   // 1: Agent
,  { 
        0x08              // Size 16x27 px, 8x27 bytes 
      , 0x1B              
      , agent0_sps_add_lo // Pointer to agent_sps split into its 2 bytes
      , agent0_sps_add_hi
      , agent0_sps_add_lo // Again to point to the first element (same pointer)
      , agent0_sps_add_hi
      , S_Active          // Status Active = 1
      , 0x00              // t=0
      , 0x64              // Energy = 100
   }
};

// Entities
#define MAX_ENTITIES 8
TEntity  m_entities[MAX_ENTITIES];
TEntity *m_entinties2Draw[MAX_ENTITIES];
u8 m_nEnt;     // Num of active entities
u8 m_nextEnt;  // Next entity id for allocation
u8 m_nEnt2Draw;// Num of entities to be drawn

///////////////////////////////////////////////////////////////
/// EM_pointToNextFreeEntitySlot
///   Points to the next free entity slot (if there is one)
///////////////////////////////////////////////////////////////
void EM_pointToNextFreeEntitySlot() {
   TEntity *e = m_entities + m_nextEnt;
   do {
      e++;
      m_nextEnt++;
   } while (e->status != S_Void && m_nextEnt < MAX_ENTITIES);
}

///////////////////////////////////////////////////////////////
/// EM_drawEntity
///   Draws an entity on the screen
///////////////////////////////////////////////////////////////
void EM_drawEntity(TEntity *e) {
   u8 *scr;
   scr = cpct_getScreenPtr(CPCT_VMEM_START, e->x, e->y);
   cpct_drawSpriteMaskedAlignedTable(e->sprite, scr, e->w, e->h, g_alphatable);
}

///////////////////////////////////////////////////////////////
/// EM_clearEntity
///   Clears an entity off the screen restoring backgrounds
///////////////////////////////////////////////////////////////
#define SCR_P1 cpctm_screenPtr(CPCT_VMEM_START,  0, 40)
#define SCR_P2 cpctm_screenPtr(CPCT_VMEM_START, 40, 40)

///////////////////////////////////////////////////////////////
/// EM_clearEntity
///   Clears an entity redrawing background over it
///////////////////////////////////////////////////////////////
void EM_clearEntity(TEntity *e) {
   LM_redrawBackgroundBox(e->ox, e->oy, e->w, e->h);
}

///////////////////////////////////////////////////////////////
/// EM_addEntity2Draw
///   Adds an entity to the draw list
///////////////////////////////////////////////////////////////
void EM_addEntity2Draw(TEntity *e) {
   m_entinties2Draw[m_nEnt2Draw] = e;
   m_nEnt2Draw++;
}

///////////////////////////////////////////////////////////////
/// EM_clearDrawEntityBuffer
///   Clears the buffer of entities marked to be drawn
///////////////////////////////////////////////////////////////
void EM_clearDrawEntityBuffer() {
   m_nEnt2Draw = 0;
}

///////////////////////////////////////////////////////////////
/// EM_draw
///   Draws all entities in the draw list
///////////////////////////////////////////////////////////////
void EM_draw() {
   u8 i;
   TEntity **e = m_entinties2Draw;

   for(i=0; i < m_nEnt2Draw; i++) {
      EM_drawEntity(*e);
      e++;
   }
}

///////////////////////////////////////////////////////////////
/// EM_clear
///   Clears all entities that must be redrawn leaving the 
///   background in their place.
///////////////////////////////////////////////////////////////
void EM_clear() {
   u8 i;
   TEntity **e = m_entinties2Draw;

   for(i=0; i < m_nEnt2Draw; i++) {
      EM_clearEntity(*e);
      e++;
   }
}

///////////////////////////////////////////////////////////////
/// EM_update
///   Updates all entities (performs their actions)
///////////////////////////////////////////////////////////////
void EM_update() {
   TEntity *e = m_entities;
   u8 i;

   for(i=0; i < m_nEnt; i++) {
      u8 a = e->nextAction;
      e->ox = e->x;
      e->oy = e->y;
      // Possible actions
      if (a) {
         EM_addEntity2Draw(e);

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
      }
      e++;
   }
}

///////////////////////////////////////////////////////////////
/// EM_initialize
///   Initialize entity manager
///////////////////////////////////////////////////////////////
void EM_initialize() {
   m_nEnt      = 0;
   m_nextEnt   = 0;
   m_nEnt2Draw = 0;
} 

///////////////////////////////////////////////////////////////
/// EM_createEntity
///   Creates a new entity
///////////////////////////////////////////////////////////////
TEntity* EM_createEntity(u8 x, u8 y, u8 entType) {
   TEntity *e = 0;
   
   // Allocate only if there is space left
   if (m_nEnt < MAX_ENTITIES) {
      // Get next entity slot
      e = m_entities + m_nextEnt;
      m_nEnt++;

      // Set up for next free entity slot
      EM_pointToNextFreeEntitySlot();

      // Draw the entity on next frame
      EM_addEntity2Draw(e);
      
      // Initial values for the entity
      cpct_memcpy(&(e->w), k_entityTypes + entType, 9);
      e->sprite = e->spriteset[0];
      e->x = e->ox = x;
      e->y = e->oy = y;
   }

   return e;
}
