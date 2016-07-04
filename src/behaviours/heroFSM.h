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

#ifndef __HERO_H__
#define __HERO_H__

#include <types.h>
#include "../entityman.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void EM_checkUserArrows(TEntity* e);
void EM_S_waitingUserInput(TEntity* e);
void EM_S_heroSetupAttack(TEntity* e);
void EM_S_walking(TEntity* e);
void EM_S_heroPerformsAttack(TEntity* e);

#endif