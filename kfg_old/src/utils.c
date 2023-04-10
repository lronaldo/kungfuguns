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
#include <cpctelera.h>
#include "utils.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void waitNVSyncs(u8 n) __z88dk_fastcall {
   do {
      cpct_waitVSYNC();
      cpct_waitHalts(2);
   } while (--n);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TWaitStatuses wait4newKeyPressed(TWaitStatuses const status) __z88dk_fastcall {
   cpct_scanKeyboard_f();
   {
    u8 pressed = cpct_isAnyKeyPressed();
    switch( status ) {
       case WS_Wait:           { if ( pressed == 0 ) { return WS_NoKeyIsPressed; } break; }
       case WS_NoKeyIsPressed: { if ( pressed != 0 ) { return WS_NewKeyPressed;  } break; }
       default:                { break; }
    }
   }
   return status;
}