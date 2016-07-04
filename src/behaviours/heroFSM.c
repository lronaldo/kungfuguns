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
#include "heroFSM.h"
#include "../entityman.h"
#include "../soundman.h"
#include "../scoreman.h"

///////////////////////////////////////////////////////////////
/// Timing configuration
///////////////////////////////////////////////////////////////

#define PUNCH_COOLDOWN_CYCLES 10
#define PUNCH_SETUP_CYCLES     4
#define BEING_HIT_COOLDOWN     8

///////////////////////////////////////////////////////////////
/// EM_createHitBow
///   Creates a new hit bow for hitting enemies
///////////////////////////////////////////////////////////////
void EM_createHitBow(u8 x, u8 y, u8 facing) {
   TEntity *ebow;
   
   // Bow is created left or right the player depending on facing
   if (facing == F_Right)
      x += 6; 
   else 
      x -= 4;

   ebow = EM_createEntity(x, y, E_HitBow);
   ebow->status = (ebow->status & 0xFE) | facing;
}

///////////////////////////////////////////////////////////////
/// EM_checkUserArrows
///   Checks if the user is pressing arrows and anotates it
///////////////////////////////////////////////////////////////
void EM_checkUserArrows(TEntity* e) {
   u8 nextAction = 0;
   
   // Interpret action requested by the user (Keyboard is read on interrupt)
   if (cpct_isKeyPressed(Key_CursorLeft)) {
      nextAction |= A_MoveLeft;
   } else if (cpct_isKeyPressed(Key_CursorRight)) {
      nextAction |= A_MoveRight;
   }
   if (cpct_isKeyPressed(Key_CursorUp)) {
      nextAction |= A_MoveUp;
   } else if (cpct_isKeyPressed(Key_CursorDown)) {
      nextAction |= A_MoveDown;
   }

   // Set next action for hero
   e->nextAction = nextAction;
}

///////////////////////////////////////////////////////////////
/// EM_S_heroBeingHit
///   Hero has been hit
///////////////////////////////////////////////////////////////
void EM_S_heroBeingHit(TEntity *e) {
   --e->t;
   if (e->t) {
      //e->nextAction = (e->status & 1) ? A_MoveRight : A_MoveLeft;
      EM_move(e);
   } else {
      // Check death
      EM_enter_waitingUserInput(e);
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_enter_beingHit
///   Enters the heroBeingHit state
///////////////////////////////////////////////////////////////
void EM_enter_heroBeingHit(TEntity *e, u8 energy, u8 facing) {
   e->t       = BEING_HIT_COOLDOWN;
   e->energy -= energy;
   e->status  = (e->status & 0xFE) | facing;
   e->sprite  = e->spriteset[5];
   e->fstate  = EM_S_heroBeingHit;
   e->nextAction = (facing & 1) ? A_MoveLeft : A_MoveRight;
   EM_addEntity2Draw(e);
   CM_addLife(-energy);
   MM_playSFX(SFX_beingHit);
}


///////////////////////////////////////////////////////////////
/// EM_S_walking
///   Entity is walking
///////////////////////////////////////////////////////////////
void EM_S_walking(TEntity* e) {
   if (cpct_isKeyPressed(Key_Space)) {
      EM_enter_heroSetupAttack(e);
   } else {
      EM_checkUserArrows(e);
      if (e->nextAction) {
         EM_nextWalkingFrame(e);
         EM_move(e);
      } else {
         EM_enter_waitingUserInput(e);
      }
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_enter_walking
///   Enters the walking state
///////////////////////////////////////////////////////////////
void EM_enter_walking(TEntity* e) {
   e->t      = 0xFF;
   e->fstate = EM_S_walking;
   EM_S_walking(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_waitingUserInput
///   Entity is waiting user input to move
///////////////////////////////////////////////////////////////
void EM_S_waitingUserInput(TEntity* e) {
   // Check if user wants to hit
   if (cpct_isKeyPressed(Key_Space)) {
      EM_enter_heroSetupAttack(e);
   } else {
      // Check if user presses arrows
      EM_checkUserArrows(e);
      if (e->nextAction)
         EM_enter_walking(e);
   }
}

///////////////////////////////////////////////////////////////
/// EM_enter_waitingUserInput
///   Enters the waiting for more user input state
///////////////////////////////////////////////////////////////
void EM_enter_waitingUserInput(TEntity* e) {
   e->t      = 0;
   e->sprite = e->spriteset[0];
   e->fstate = EM_S_waitingUserInput;
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_heroPerformsAttack
///   State: hero is performing its attack
///////////////////////////////////////////////////////////////
void EM_S_heroPerformsAttack(TEntity *e) {
   // Wait for cooldown
   --e->t;
   if (!e->t)
      EM_enter_waitingUserInput(e);
}

///////////////////////////////////////////////////////////////
/// EM_createHitBow
///   Creates a new hit bow for hitting enemies
///////////////////////////////////////////////////////////////
void EM_enter_heroPerformsAttack(TEntity *e) {
   TPoint  *p = e->pos + 2;

   // Perform attack
   EM_createHitBow(p->x, p->y + 2, (e->status & 1));
   MM_playSFX(SFX_punch);

   // Enter the performs attack state   
   e->t      = PUNCH_COOLDOWN_CYCLES;
   e->sprite = e->spriteset[4];
   e->fstate = EM_S_heroPerformsAttack;

   EM_addEntity2Draw(e);
}
 
///////////////////////////////////////////////////////////////
/// EM_S_heroSetupAttack
///   State: hero is waiting for attacking
///////////////////////////////////////////////////////////////
void EM_S_heroSetupAttack(TEntity *e) {  
   --e->t;
   if (!e->t)
      EM_enter_heroPerformsAttack(e);
}

///////////////////////////////////////////////////////////////
/// EM_hero_enter_setupAttack
///   Enters the setupAttack State
///////////////////////////////////////////////////////////////
void EM_enter_heroSetupAttack(TEntity *e) {
   // Start hero attack
   e->t      = PUNCH_SETUP_CYCLES;
   e->sprite = e->spriteset[3];
   e->fstate = EM_S_heroSetupAttack;
   EM_addEntity2Draw(e);
}
