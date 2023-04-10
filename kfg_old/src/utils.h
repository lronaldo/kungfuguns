//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Kung Fu Guns: A CPCtelera game made for #bitbitjam3
//  Copyright (C) 2023 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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
#ifndef __UTILS_H__
#define __UTILS_H__

#include <types.h>

typedef enum {
   WS_Wait = 0,
   WS_NoKeyIsPressed = 1,
   WS_NewKeyPressed = 2
} TWaitStatuses;

void           waitNVSyncs(u8 n) __z88dk_fastcall;
TWaitStatuses  wait4newKeyPressed(TWaitStatuses const status) __z88dk_fastcall;

#endif