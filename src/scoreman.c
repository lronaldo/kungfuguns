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
#include <stdio.h>
#include "scoreman.h"
#include "sprites/scorepiece.h" 
#include "sprites/drawSpriteFlippedTable.h" 

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// DATA
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

u8  mcm_life;
u8  mcm_oldlife;
u16 mcm_points;
u8  mcm_status;
u8  mcm_str_points[6];

#define LIFEBAR_X 28
#define LIFEBAR_Y  8
#define ID_BRIGHT_RED   0x03
#define ID_BLUE         0xF0

// Status update flags
typedef enum {
      MS_fullredraw   = 0x01
   ,  MS_updatepoints = 0x02
   ,  MS_updatelife   = 0x04
} TMCMStatus;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// FUNCTIONS
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
/// CM_drawHeader
///   Draws the header of the scoreboard
///////////////////////////////////////////////////////////////
void CM_drawHeader() {
   // Draw header sprites
   drawSpriteFlipAlphaTable(g_scorepiece, CPCT_VMEM_START +  8, 16, 36);
   drawSpriteFlipAlphaTable(g_scorepiece, (u8*)0x8000     +  8, 16, 36);
   switchFlipForDrawSprite();
   drawSpriteFlipAlphaTable(g_scorepiece, CPCT_VMEM_START + 56, 16, 36);
   drawSpriteFlipAlphaTable(g_scorepiece, (u8*)0x8000     + 56, 16, 36);
   switchFlipForDrawSprite();
}

///////////////////////////////////////////////////////////////
/// CM_initialize
///   Initializes the score manager
///////////////////////////////////////////////////////////////
void CM_inititalize(u8 life, u16 points) {
   mcm_life     = life;
   mcm_oldlife  = life;  
   mcm_status   = MS_fullredraw;
   CM_setPoints(points);
}

///////////////////////////////////////////////////////////////
/// CM_setPoints
///   Sets new score and creates associated string
///////////////////////////////////////////////////////////////
void CM_setPoints(u16 points) {
   mcm_points  = points;
   mcm_status |= MS_updatepoints;
   sprintf(mcm_str_points, "%05d", points);
}

///////////////////////////////////////////////////////////////
/// CM_addPoints
///   Adds more points to the score
///////////////////////////////////////////////////////////////
void CM_addPoints(u16 points) {
   CM_setPoints(mcm_points + points);
}

///////////////////////////////////////////////////////////////
/// CM_addLife
///   Add life points
///////////////////////////////////////////////////////////////
void CM_addLife(i8 lifeadd) {
   mcm_life = mcm_life + lifeadd;
   if (mcm_life > 24) {
      if (lifeadd > 0)
         mcm_life = 24;
      else
         mcm_life = 0;
   }
   mcm_status |= MS_updatelife;
}

///////////////////////////////////////////////////////////////
/// CM_getLife
///   Gets the remaining life
///////////////////////////////////////////////////////////////
u8  CM_getLife() {
   return mcm_life; 
}

///////////////////////////////////////////////////////////////
/// CM_drawDualBox
///   Draws a solid box on both screen buffers
///////////////////////////////////////////////////////////////
void CM_drawDualBox(u8 x, u8 size, u8 color) {
   cpct_drawSolidBox(cpctm_screenPtr(CPCT_VMEM_START, x, 8), color, size, 8);
   cpct_drawSolidBox(cpctm_screenPtr(         0x8000, x, 8), color, size, 8);  
}

///////////////////////////////////////////////////////////////
/// CM_fullredraw
///   Redraws all the scoreboard (life and points)
///////////////////////////////////////////////////////////////
void CM_fullredraw() {
   u8 red  = (mcm_life > 24) ? 24 : mcm_life;
   u8 blue;

   // Calculate blue
   if (red < 24)
      blue = 24 - red;
   else
      blue = 0;

   // Draw Life bar
   if (red)
      CM_drawDualBox(LIFEBAR_X      ,  red, ID_BRIGHT_RED);
   if (blue)
      CM_drawDualBox(LIFEBAR_X + red, blue, ID_BLUE);
   mcm_oldlife = mcm_life;

   // Also update points and header
   CM_updatePoints_draw();
   CM_drawHeader();
   mcm_status = 0;
}

///////////////////////////////////////////////////////////////
/// CM_updatePoints_draw
///   Updates points drawing two strings
///////////////////////////////////////////////////////////////
void CM_updateLife_draw() {
   if (mcm_oldlife > mcm_life) {
      CM_drawDualBox(LIFEBAR_X +    mcm_life, mcm_oldlife - mcm_life, ID_BLUE);
   } else if (mcm_oldlife < mcm_life) {
      CM_drawDualBox(LIFEBAR_X + mcm_oldlife, mcm_life - mcm_oldlife, ID_BRIGHT_RED);
   }
   mcm_oldlife = mcm_life;
}

///////////////////////////////////////////////////////////////
/// CM_updatePoints_draw
///   Updates points drawing two strings
///////////////////////////////////////////////////////////////
void CM_updatePoints_draw() {
   cpct_drawStringM0(mcm_str_points, cpctm_screenPtr(CPCT_VMEM_START, 30, 24), 7, 5);
   cpct_drawStringM0(mcm_str_points, cpctm_screenPtr(         0x8000, 30, 24), 7, 5); 
}

///////////////////////////////////////////////////////////////
/// CM_updateDraw
///   Only draws parts that have changed
///////////////////////////////////////////////////////////////
void CM_updateDraw() {
   if (mcm_status & MS_updatelife)
      CM_updateLife_draw();

   if (mcm_status & MS_updatepoints)
      CM_updatePoints_draw();
}

///////////////////////////////////////////////////////////////
/// CM_initialize
///   Initializes the score manager
///////////////////////////////////////////////////////////////
void CM_draw() {
   if (mcm_status & MS_fullredraw) 
      CM_fullredraw();
   else 
      CM_updateDraw();
   mcm_status = 0;
}
