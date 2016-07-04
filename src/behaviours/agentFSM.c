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
/// EM_S_beingHit
///   Enters the beingHit state
///////////////////////////////////////////////////////////////
void EM_S_beingHit(TEntity *e) {
   --e->t;
   if (e->t) {
      //e->nextAction = (e->status & 1) ? A_MoveRight : A_MoveLeft;
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
void EM_S_enter_beingHit(TEntity *e, u8 energy, u8 facing) {
   e->t       = 8;
   e->energy -= energy;
   e->status  = (e->status & 0xFE) | facing;
   e->sprite  = e->spriteset[3];
   e->fstate  = EM_S_beingHit;
   e->nextAction = (facing & 1) ? A_MoveLeft : A_MoveRight;
   EM_addEntity2Draw(e);
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
/// EM_processAI
///   Process AI of the entity
///////////////////////////////////////////////////////////////
void EM_processAI(TEntity *e) {
   e->t      = 0;
   e->fstate = EM_S_pursuitHero;
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
   if (p->x < herop->x - 7) {
      a = A_MoveRight;
   } else if (p->x > herop->x + 7) {
      a = A_MoveLeft;
   } else if (p->y < herop->y) {
      a = A_MoveDown;
   } else if (p->y > herop->y) {
      a = A_MoveUp;
   } else {
      // Change to patrol state
   }

   // If agent is moving
   if (a) {
      e->nextAction = a;
      EM_nextWalkingFrame(e);
      EM_move(e);
      EM_addEntity2Draw(e);
   }
}
