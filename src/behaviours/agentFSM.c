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
#include "agentFSM.h"
#include "../entityman.h"
#include "../soundman.h"

///////////////////////////////////////////////////////////////
/// Timing configuration
///////////////////////////////////////////////////////////////

#define AGENT_PUNCH_COOLDOWN_CYCLES  8
#define AGENT_SETUP_PUNCH_CYCLES     4
#define AGENT_HIT_RADIUS_Y           1
#define AGENT_HIT_ENERGY             5

///////////////////////////////////////////////////////////////
/// EM_distance
///   Returns the linear distance between 2 points in 1 dimension
///////////////////////////////////////////////////////////////
u8 EM_distance(u8 a, u8 b) {
   if (a > b)
      return a - b;
   else
      return b - a;
}

///////////////////////////////////////////////////////////////
/// EM_checkHeroPunchDamage
///   Checks if the punch has hit hero
///////////////////////////////////////////////////////////////
void EM_checkHeroPunchDamage(TEntity *e) {
   TEntity *hero = EM_getHero();
   TPoint    *ph = hero->pos + 2;
   TPoint    *pe = e->pos + 2;
   u8          d;

   // Get y distance
   d = EM_distance(pe->y, ph->y);

   // Only hit on distance <= 1
   if (d <= AGENT_HIT_RADIUS_Y) {
      // Get x distance and facing
      u8 facing = e->status & 1;
      d = EM_distance(pe->x, ph->x);
     
      // Punch left or right
      if (     facing == F_Left 
            && d <= hero->w + 1
            && ph->x < pe->x
         ) {
         EM_enter_heroBeingHit(hero, AGENT_HIT_ENERGY, F_Left);
      } else if ( facing == F_Right
               && d <= e->w + 1
               && ph->x > pe->x ) {
         EM_enter_heroBeingHit(hero, AGENT_HIT_ENERGY, F_Right);
      }
   }
}

///////////////////////////////////////////////////////////////
/// EM_S_beingHit
///   Enters the beingHit state
///////////////////////////////////////////////////////////////
void EM_S_beingHit(TEntity *e) {
   --e->t;
   if (e->t) {
      //e->nextAction = (e->status & 1) ? A_MoveRight : A_MoveLeft;
      EM_move(e);
   } else {
      EM_enter_processAI(e);
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_enter_beingHit
///   Enters the beingHit state
///////////////////////////////////////////////////////////////
void EM_S_enter_beingHit(TEntity *e, u8 energy, u8 facing) {
   e->t       = 8;
   e->energy -= energy;
   e->status  = (e->status & 0xFE) | facing;
   e->sprite  = e->spriteset[5];
   e->fstate  = EM_S_beingHit;
   e->nextAction = (facing & 1) ? A_MoveLeft : A_MoveRight;
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_setupPunch
///   Punching and coolding down
///////////////////////////////////////////////////////////////
void EM_S_punch(TEntity *e) {
   --e->t;
   if (!e->t) {
      EM_enter_processAI(e);
   } else {
      EM_checkHeroPunchDamage(e);
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_enter_punch
///   Agent is punching the hero
///////////////////////////////////////////////////////////////
void EM_enter_punch(TEntity *e) {
   e->t      = AGENT_PUNCH_COOLDOWN_CYCLES;
   e->fstate = EM_S_punch;
   e->sprite = e->spriteset[4];
   EM_addEntity2Draw(e);   
}

///////////////////////////////////////////////////////////////
/// EM_S_setupPunch
///   Preparing for punching
///////////////////////////////////////////////////////////////
void EM_S_setupPunch(TEntity *e) {
   --e->t;
   if (!e->t)
      EM_enter_punch(e);
}

///////////////////////////////////////////////////////////////
/// EM_enter_setupPunch
///   Agent is preparing for punching the hero
///////////////////////////////////////////////////////////////
void EM_enter_setupPunch(TEntity *e) {
   e->t      = AGENT_SETUP_PUNCH_CYCLES;
   e->fstate = EM_S_setupPunch;
   e->sprite = e->spriteset[3];
   EM_addEntity2Draw(e);   
}

///////////////////////////////////////////////////////////////
/// EM_processAI
///   Process AI of the entity
///////////////////////////////////////////////////////////////
void EM_processAI(TEntity *e) {
   e->t      = 0;
   e->fstate = EM_S_pursuitHero;
}

///////////////////////////////////////////////////////////////
/// EM_enter_processAI
///   An entity returns to the processAI state
///////////////////////////////////////////////////////////////
void EM_enter_processAI(TEntity *e) {
   e->sprite = e->spriteset[0];
   e->fstate = EM_processAI;
   if (e->energy < 0)
      EM_deleteEntity(e);
   EM_addEntity2Draw(e);   
}

///////////////////////////////////////////////////////////////
/// EM_S_pursuitHero
///   Pursues hero up to being in from of him
///////////////////////////////////////////////////////////////
void EM_S_pursuitHero(TEntity *e) {
   // Get hero
   TPoint *herop = EM_getHero()->pos + 2;
   TPoint     *p = e->pos + 2;
   u8          a = 0;
   
   // Follow hero
   if (p->x < herop->x - 6) {
      a = A_MoveRight;
   } else if (p->x > herop->x + 6) {
      a = A_MoveLeft;
   } else if (p->y < herop->y) {
      a = A_MoveDown;
   } else if (p->y > herop->y) {
      a = A_MoveUp;
   } else {
      EM_enter_setupPunch(e);
   }

   // If agent is moving
   if (a) {
      e->nextAction = a;
      EM_nextWalkingFrame(e);
      EM_move(e);
      EM_addEntity2Draw(e);
   }
}
