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
#include "gameman.h"
#include "levelman.h"
#include "sprites/princess.h"
#include "sprites/agent0.h"
#include "sprites/spritesets.h"

// Print transparency mask table
cpctm_createTransparentMaskTable(g_alphatable, 0x100, M0, 0);

// Entity types
#define NUM_ENTITY_ATTRIBS 8
const u8 k_entityTypes[2][NUM_ENTITY_ATTRIBS] = {
   // 0: Princess
   { 
        0x06                  // Size 12x28 px, 6x28 bytes 
      , 0x1C              
      , princess_sps_add_lo   // Pointer to princess_sps split into its 2 bytes
      , princess_sps_add_hi
      , princess_sps_add_lo   // Again to point to the first element (same pointer)
      , princess_sps_add_hi
      , 0x00                  // t=0
      , 0x64                  // Energy = 100
   }
   // 1: Agent
,  { 
        0x06              // Size 12x28 px, 6x28 bytes 
      , 0x1C              
      , agent0_sps_add_lo // Pointer to agent_sps split into its 2 bytes
      , agent0_sps_add_hi
      , agent0_sps_add_lo // Again to point to the first element (same pointer)
      , agent0_sps_add_hi
      , 0x00              // t=0
      , 0x64              // Energy = 100
   }
};

// Entities
#define MAX_ENTITIES 8
TEntity  m_entities[MAX_ENTITIES];
TEntity *m_entinties2Draw[MAX_ENTITIES];
u8 m_nEnt;     // Num of active entities
u8 m_nEnt2Draw;// Num of entities to be drawn

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
///   Clears an entity redrawing background over it
///////////////////////////////////////////////////////////////
#define SCR_P1 cpctm_screenPtr(CPCT_VMEM_START,  0, 40)
#define SCR_P2 cpctm_screenPtr(CPCT_VMEM_START, 40, 40)
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
   TEntity **e = m_entinties2Draw;
   u8        i = m_nEnt2Draw;

   while(i) {
      EM_drawEntity(*e);
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_clear
///   Clears all entities that must be redrawn leaving the 
///   background in their place.
///////////////////////////////////////////////////////////////
void EM_clear() {
   TEntity **e = m_entinties2Draw;
   u8        i = m_nEnt2Draw;

   while(i) {
      EM_clearEntity(*e);
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_performActions
///   Perform actions of a given entity
///////////////////////////////////////////////////////////////
void EM_performActions(TEntity *e) {
   u8 a = e->nextAction;

   // Ensure previous movements are cancelled
   e->ox = e->x;
   e->oy = e->y;
   
   // Possible actions
   if (a) {
      EM_addEntity2Draw(e);

      // Left - Right
      if (a & A_MoveLeft  && e->x > 0) {
         e->x--; 
      } else if (a & A_MoveRight && e->x < 80 - e->w) {
         e->x++; 
      } 

      // Up - Down
      if (a & A_MoveUp    && e->y > 108 - e->h) {
         e->y--; 
      } else if (a & A_MoveDown  && e->y < 200 - e->h) {
         e->y++; 
      }
   }
}

///////////////////////////////////////////////////////////////
/// EM_processAI
///   Process AI of the entity
///////////////////////////////////////////////////////////////
void EM_processAI(TEntity *e) {
   // Get hero
   TEntity *hero = m_entities;
   u8 a = 0;
   
   // Follow hero
   if (e->x < hero->x - 7) {
      a = A_MoveRight;
   } else if (e->x > hero->x + 7) {
      a = A_MoveLeft;
   } else if (e->y < hero->y) {
      a = A_MoveDown;
   } else if (e->y > hero->y) {
      a = A_MoveUp;
   }
   e->nextAction = a;
}

///////////////////////////////////////////////////////////////
/// EM_update
///   Updates all entities (performs their actions)
///////////////////////////////////////////////////////////////
void EM_update() {
   TEntity *e = m_entities;
   u8 i = m_nEnt;

   while(i) {
      e->fstate(e);
      EM_performActions(e);
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_initialize
///   Initialize entity manager
///////////////////////////////////////////////////////////////
void EM_initialize() {
   m_nEnt      = 0;
   m_nEnt2Draw = 0;
} 

///////////////////////////////////////////////////////////////
/// EM_createEntity
///   Creates a new entity
///////////////////////////////////////////////////////////////
TEntity* EM_createEntity(u8 x, u8 y, u8 entType) {
   TEntity *e = 0;
   
   // Allocate only if there is space left
   if (m_nEnt < MAX_ENTITIES - 1) {
      // Get next entity slot
      e = m_entities + m_nEnt;
      m_nEnt++;

      // Draw the entity on next frame
      EM_addEntity2Draw(e);
      
      // Initial values for the entity
      cpct_memcpy(&(e->w), k_entityTypes + entType, NUM_ENTITY_ATTRIBS);
      e->sprite = e->spriteset[0];
      e->fstate = (m_nEnt == 1) ? GM_getUserInput : EM_processAI;
      e->x = e->ox = x;
      e->y = e->oy = y;
   }

   return e;
}

///////////////////////////////////////////////////////////////
/// EM_moveEntityX
///   Moves an entity in the X axis leaving it always inside
///   screen boundaries. Sets it for redraw after moving
///////////////////////////////////////////////////////////////
void EM_moveEntityX(TEntity* e, i8 pixels) {
   e->x += pixels;
   if (e->x > 80 - e->w) {
      if (pixels < 0) 
         e->x = 0;
      else
         e->x = 80 - e->w;
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_scroll
///   Scrolls (moves horizontally) all entities
///////////////////////////////////////////////////////////////
void EM_scroll(i8 pixels) {
   TEntity *e = m_entities;
   u8 i = m_nEnt;

   while (i) {
      EM_moveEntityX(e, pixels);
      ++e; --i;
   }
}