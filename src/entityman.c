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
#include "screenman.h"
#include "levelman.h"
#include "heroFSM.h"
#include "sprites/princess.h"
#include "sprites/agent0.h"
#include "sprites/hit.h"
#include "sprites/spritesets.h"
#include "sprites/drawSpriteFlippedTable.h"

// Print transparency mask table
cpctm_createTransparentMaskTable(g_alphatable, 0x200, M0, 0);

// Entity types
#define NUM_ENTITY_ATTRIBS 9
const u8 k_entityTypes[3][NUM_ENTITY_ATTRIBS] = {
   // 0: Princess
   { 
        0x06                  // Size 12x28 px, 6x28 bytes 
      , 0x1C
      , 0x02                  // New entity, has to be drawn twice (once per buffer)
      , T_Princess            // Type of entity: princess
      , F_Right               // Entity is facing right
      , princess_sps_add_lo   // Pointer to princess_sps split into its 2 bytes
      , princess_sps_add_hi
      , 0x00                  // t=0
      , 0x64                  // Energy = 100
   }
   // 1: Agent
,  { 
        0x06              // Size 12x28 px, 6x28 bytes 
      , 0x1C
      , 0x02              // New entity, has to be drawn twice (once per buffer)
      , T_Agent           // Type of entity: agent
      , F_Left            // Entity is facing left
      , agent0_sps_add_lo // Pointer to agent_sps split into its 2 bytes
      , agent0_sps_add_hi
      , 0x00              // t=0
      , 0x09              // Energy = 9
   }
   // 2: Hit Bow
,  { 
        0x04                  // Size 8x14 px, 4x14 bytes 
      , 0x0E
      , 0x02                  // New entity, has to be drawn twice (once per buffer)
      , T_HitBow              // Type of entity: Hit Bow
      , F_Right               // Entity is facing right
      , heroAttack_sps_add_lo // Attack has only 1 sprite (no spriteset)
      , heroAttack_sps_add_hi
      , 0x03                  // t=3 frames
      , 0x03                  // Energy = 3
   }
};

// Entities
#define MAX_ENTITIES 8
TEntity m_entities[MAX_ENTITIES];
extern  u8 m_nEnt;        // Num of active entities
extern  u8 m_nFrame;      // Update frame counter
extern  u8 m_statusFlags; // Status flags: 0: drawSpriteFacing (0 Right, 1 Left)


///////////////////////////////////////////////////////////////
/// dummy_init
///   Statically initialize variables
///////////////////////////////////////////////////////////////
void EM_dummy_init() {
   __asm
      _m_nEnt::        .db 0
      _m_nFrame::      .db 0
      _m_statusFlags:: .db 0
   __endasm;
}

///////////////////////////////////////////////////////////////
/// EM_drawEntity
///   Draws an entity on the screen (only if they are not 
///   destroyed, type = 0)
///////////////////////////////////////////////////////////////
void EM_drawEntity(TEntity *e, void* buf) {
   TPoint* p = e->pos + 2;
   u8 *scr   = cpct_getScreenPtr(buf, p->x, p->y);

   // Switch drawing mode if required
   if ( (e->status & 1) != (m_statusFlags & 1) ) {
      switchFlipForDrawSprite();
      m_statusFlags ^= 1;
   }

   // Draw Entity
   drawSpriteFlipAlphaTable(e->sprite, scr, e->w, e->h);
}

///////////////////////////////////////////////////////////////
/// EM_clearEntity
///   Clears an entity redrawing background over it
///////////////////////////////////////////////////////////////
void EM_clearEntity(TEntity *e, void *buf) {
   TPoint* p = e->pos + 0;
   u8      i = 4;
   LM_redrawBackgroundBox(p->x, p->y, e->w, e->h, buf);

   // pos[0] = pos[1] and pos[1] = pos[2]
   // Copy bytes overwritting pos[0] which is outdated
   *((u32*)p) = *((u32*)(p+1));

   // After clearing, previous location does not matter anymore
   //cpct_memcpy(p, p+1, 2*sizeof(TPoint));
   
   //while (i) {
   //   *((u16*)p) = *((u16*)(p+1));
   //   ++p; --i;
   //}
}

///////////////////////////////////////////////////////////////
/// EM_addEntity2Draw
///   Adds an entity to the draw list (if it is not there already)
///   of one of the draw buffers for next frames
///////////////////////////////////////////////////////////////
void EM_addEntity2Draw(TEntity *e2d) {
   e2d->drawt = 2;
}

///////////////////////////////////////////////////////////////
/// EM_draw
///   Draws all entities in the draw list (in the backbuffer)
///////////////////////////////////////////////////////////////
void EM_draw() {
   TEntity  *e = m_entities;
   void   *buf = SM_backBuf();
   u8        i = m_nEnt;

   while(i) {
      if ( e->drawt && e->type) {
         EM_drawEntity(e, buf);
         --e->drawt;
      }
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_clear
///   Clears all entities that must be redrawn leaving the 
///   background in their place.
///////////////////////////////////////////////////////////////
void EM_clear() {
   TEntity  *e = m_entities;
   void   *buf = SM_backBuf();
   u8        i = m_nEnt;

   while(i) {
      if ( e->drawt || !e->type )
         EM_clearEntity(e, buf);
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_move
///   Moves an entity in X and Y
///////////////////////////////////////////////////////////////
void EM_move(TEntity *e) {
   TPoint *p = e->pos + 2;
   u8      a = e->nextAction;
  
   // Left - Right
   if ( (a & A_MoveLeft) && p->x > 0) {
      p->x--;
      e->status = (e->status & 0xFE) | F_Left;
   } else if ( (a & A_MoveRight) && p->x < 80 - e->w) {
      p->x++; 
      e->status = (e->status & 0xFE) | F_Right;
   } 

   // Up - Down
   if ( (a & A_MoveUp) && p->y > 108 - e->h) {
      p->y--; 
   } else if ( (a & A_MoveDown) && p->y < 200 - e->h) {
      p->y++; 
   }
}

///////////////////////////////////////////////////////////////
/// EM_processAI
///   Process AI of the entity
///////////////////////////////////////////////////////////////
void EM_processAI(TEntity *e) {
   // Get hero
   TPoint *herop = m_entities[0].pos + 2;
   TPoint     *p = e->pos + 2;
   u8          a = 0;
   
   // Follow hero
   if (p->x < herop->x - 7) {
      a = A_MoveRight;
   } else if (p->x > herop->x + 7) {
      a = A_MoveLeft;
   } else if (p->y < herop->y) {
      a = A_MoveDown;
   } else if (p->y > herop->y) {
      a = A_MoveUp;
   }
   if (a) {
      e->nextAction = a;
      EM_move(e);
      EM_addEntity2Draw(e);
   }
}

///////////////////////////////////////////////////////////////
/// EM_freeDeletedEntity
///   Frees space that a deleted entity takes
///////////////////////////////////////////////////////////////
void EM_freeDeletedEntity(TEntity *e) {
   TEntity *last = m_entities + m_nEnt - 1;

   // Free the entity overwritting it with the last one
   if (e != last)
      cpct_memcpy(e, last, sizeof(TEntity));

   // Set up remaining entities value
   m_nEnt--;
}

///////////////////////////////////////////////////////////////
/// EM_update
///   Updates all entities (performs their actions)
///////////////////////////////////////////////////////////////
void EM_update() {
   TEntity *e = m_entities;
   u8 i = m_nEnt;
   u8 f = m_nFrame++ & 1;  // A new update frame has passed

   // Hero updates without checking LOD (hero is always entity 0)
   goto heroupdatefirst;

   // Update all entities
   while(i) {
      // Update half entities depending on frame
      if ( (i & 1) == f) {
         heroupdatefirst:
         e->fstate(e);
      }
      ++e; --i;
   }
}

///////////////////////////////////////////////////////////////
/// EM_S_waitingDelete
///   An entity is wainting to be deleted because it has to be
///   removed from backbuffers
///////////////////////////////////////////////////////////////
void EM_S_waitingDelete(TEntity *e) {
   --e->t;
   if (!e->t)
      EM_freeDeletedEntity(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_enter_processAI
///   An entity returns to the processAI state
///////////////////////////////////////////////////////////////
void EM_S_enter_processAI(TEntity *e) {
   e->sprite = e->spriteset[0];
   e->fstate = EM_processAI;
   if (e->energy < 0)
      EM_deleteEntity(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_beingHit
///   Enters the beingHit state
///////////////////////////////////////////////////////////////
void EM_S_beingHit(TEntity *e) {
   --e->t;
   if (e->t) {
      e->nextAction = A_MoveRight;
      EM_move(e);
   } else {
      EM_S_enter_processAI(e);
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_enter_beingHit
///   Enters the beingHit state
///////////////////////////////////////////////////////////////
void EM_S_enter_beingHit(TEntity *e, u8 energy) {
   e->t       = 8;
   e->energy -= energy;
   e->sprite  = e->spriteset[3];
   e->fstate  = EM_S_beingHit;
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_deleteEntity
///   Marks an entity to be destroyed
///////////////////////////////////////////////////////////////
void EM_deleteEntity(TEntity *e) {
   e->t      = 2;    // 2 cicles until deleting
   e->type   = T_Destroyed;
   e->fstate = EM_S_waitingDelete;
}

///////////////////////////////////////////////////////////////
/// EM_S_hitEnemy
///   A Hit Bow checks if it is hitting an enemy or not
///////////////////////////////////////////////////////////////
void EM_S_hitEnemy(TEntity *ebow) {
   TPoint  *pbow = ebow->pos + 2;
   TEntity *e    = m_entities + 1;
   u8       i    = m_nEnt - 1;

   // Check against all enemies
   while (i) {
      TPoint  *pe = e->pos + 2;
      // Only for enemies
      if ( (e->type & T_Agent) && e->fstate != EM_S_beingHit ) {
         if ( pe->y == pbow->y - 2) {
            if (  pe->x > pbow->x - e->w
               && pe->x < pbow->x + ebow->w)
               EM_S_enter_beingHit(e, ebow->energy);
         }
      }
      --i; ++e;
   }

   // Always redraw
   EM_addEntity2Draw(ebow);

   // Count down
   --ebow->t;
   if (!ebow->t)
      EM_deleteEntity(ebow);
}

///////////////////////////////////////////////////////////////
/// EM_createEntity
///   Creates a new entity
///////////////////////////////////////////////////////////////
TEntity* EM_createEntity(u8 x, u8 y, u8 entityID) {
   TEntity *e = 0;
   
   // Allocate only if there is space left
   if (m_nEnt < MAX_ENTITIES - 1) {
      void *fsm;
      TPoint *p;

      // Get next entity slot
      e = m_entities + m_nEnt;
      m_nEnt++;

      // Draw the entity on next frame
      EM_addEntity2Draw(e);
      
      // Initial values for the entity
      cpct_memcpy(&(e->w), k_entityTypes + entityID, NUM_ENTITY_ATTRIBS);
      e->sprite = e->spriteset[0];
      p = e->pos + 0;
      p->x = x; p->y = y; p++;
      p->x = x; p->y = y; p++;
      p->x = x; p->y = y; p++;

      // Assign FSM
      switch(entityID) {
         case E_Princess: fsm = EM_S_waitingUserInput; break;
         case E_Agent:    fsm = EM_processAI; break;
         case E_HitBow:   fsm = EM_S_hitEnemy; break;
      }
      e->fstate = fsm;
   }

   return e;
}

///////////////////////////////////////////////////////////////
/// EM_moveEntityX
///   Moves an entity in the X axis leaving it always inside
///   screen boundaries. Sets it for redraw after moving
///////////////////////////////////////////////////////////////
void EM_moveEntityX(TEntity* e, i8 pixels) {
   // Calculations
   TPoint *p = e->pos + 2;  
   u8 limit  = 80 - e->w;
   u8 nx     = p->x + pixels;

   // Ensure boundaries
   if (nx > limit) {
      if (pixels < 0) 
         nx = 0;
      else
         nx = limit;
   }

   // Move entity
   p->x = nx;
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