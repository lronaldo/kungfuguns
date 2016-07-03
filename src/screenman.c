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

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

extern void* m_backbuffer;
extern void* m_screenbuffer;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/// dummy_init
///   Dummy function to have preinitialized values
///////////////////////////////////////////////////////////////
void dummy_init() __naked {
   __asm
      _m_backbuffer::   .dw 0x8000
      _m_screenbuffer:: .dw 0xC000
   __endasm;
}

///////////////////////////////////////////////////////////////
/// SM_cpc_showBackBuffer
///   Makes the screen change to the actual hardware backbuffer
///////////////////////////////////////////////////////////////
void SM_cpc_showBackBuffer() {
   u8 buff;

   if (m_backbuffer == CPCT_VMEM_START) {
      buff = cpct_pageC0;
   } else {
      buff = cpct_page80;
   }

   cpct_setVideoMemoryPage(buff);
}

///////////////////////////////////////////////////////////////
/// SM_switchScreenBuffers
///   switches hardware backbuffer and screen buffer
///////////////////////////////////////////////////////////////
void SM_switchScreenBuffers() {
   void *aux;

   SM_cpc_showBackBuffer();

   // Switch pointer values
   aux            = (void*)m_backbuffer;
   m_backbuffer   = m_screenbuffer;
   m_screenbuffer = aux;
}

///////////////////////////////////////////////////////////////
/// SM_backBuf
///   returns a pointer to the start of the video memory back buffer
///////////////////////////////////////////////////////////////
void* SM_backBuf() { return m_backbuffer; }

///////////////////////////////////////////////////////////////
/// SM_scrBuf
///   returns a pointer to the start of the current video memory buffer
///////////////////////////////////////////////////////////////
void* SM_scrBuf() { return m_screenbuffer; }
