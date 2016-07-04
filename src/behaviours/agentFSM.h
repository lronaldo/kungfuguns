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

#ifndef __AGENTFSM_H__
#define __AGENTFSM_H__

#include <types.h>
#include "../entityman.h"

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void EM_S_beingHit(TEntity *e);
void EM_S_enter_beingHit(TEntity *e, u8 energy, u8 facing);
void EM_S_pursuitHero(TEntity *e);
void EM_S_heroBeingHit(TEntity *e);
void EM_enter_heroBeingHit(TEntity *e, u8 energy, u8 facing);
void EM_enter_processAI(TEntity *e);
void EM_processAI(TEntity *e);


#endif