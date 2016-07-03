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
#include "entityman.h"

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
/// EM_S_walking
///   Entity is walking
///////////////////////////////////////////////////////////////
#define FRAMES_STEP_WALK   2
void EM_S_walking(TEntity* e) {
   EM_checkUserArrows(e);
   if (e->nextAction) {
      // Next animation frame
      u8 ft = ++e->t / FRAMES_STEP_WALK;
      if (ft >= 3)
         e->t = FRAMES_STEP_WALK;
      e->sprite = e->spriteset[ft];
      EM_move(e);
   } else {
      // Stop walking
      e->sprite = e->spriteset[1];
      e->fstate = EM_S_waitingUserInput;
   }
   EM_addEntity2Draw(e);
}

///////////////////////////////////////////////////////////////
/// EM_S_waitingUserInput
///   Entity is waiting user input to move
///////////////////////////////////////////////////////////////
void EM_S_waitingUserInput(TEntity* e) {
   // Check if user wants to hit
   if (cpct_isKeyPressed(Key_Space)) {
      // Start hero attack
      e->t      = 2;   // 2 frames for set up
      e->sprite = e->spriteset[5];
      e->fstate = EM_S_heroSetupAttack;
      EM_addEntity2Draw(e);
   } else {
      // Check if user presses arrows
      EM_checkUserArrows();
      if (e->nextAction) {
         e->t      = FRAMES_STEP_WALK - 1;
         e->fstate = EM_S_walking;
         EM_S_walking(e);
      }
   }
}

///////////////////////////////////////////////////////////////
/// EM_S_heroPerformsAttack
///   State: hero is performing its attack
///////////////////////////////////////////////////////////////
void EM_S_heroPerformsAttack(TEntity *e) {
   --e->t;
   if (!e->t) {
      // Return to normal state
      e->fstate = EM_S_waitingUserInput;
      e->sprite = e->spriteset[1];
      EM_addEntity2Draw(e);     
   }
}

///////////////////////////////////////////////////////////////
/// EM_S_heroSetupAttack
///   State: hero is waiting for attacking
///////////////////////////////////////////////////////////////
void EM_S_heroSetupAttack(TEntity *e) {
   if (cpct_isKeyPressed(Key_Space)) {
      --e->t;
      if (!e->t) {
         // Perform attack
         e->t = 2;
         e->sprite = e->spriteset[6];
         e->fstate = EM_S_heroPerformsAttack;
         EM_addEntity2Draw(e);
      }
   } else {
      // Return to normal state
      e->fstate = EM_S_waitingUserInput;
      e->sprite = e->spriteset[1];
      EM_addEntity2Draw(e);
   }
}