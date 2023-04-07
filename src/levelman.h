//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Kung Fu Guns: A CPCtelera game made for #bitbitjam3
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

#include <types.h>

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// EXPORTED PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void  LM_initialize(u8 level);
void  LM_redrawBackgroundBox(u8 x, u8 y, u8 w, u8 h, u8* buf);
void  LM_update(u8 hero_x);
void  LM_draw();
void  LM_drawBg(void* buf);
u8    LM_isLevelFinished();
void  LM_setOffset(u8 offset);