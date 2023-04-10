#include <cpctelera.h>
#include <assets/g_palette.h>
#include <assets/princess.h>
#include "utils.h"

void init() {
   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setPalette(g_palette, 16);
   cpct_setBorder(HW_LIME);
}

void game() {
   u8* const p_string = cpctm_screenPtr(0xC000,  0,  96); 
   u8* const p_princ  = cpctm_screenPtr(0xC000, 38, 140);

   cpct_drawSprite(sp_princess_0, p_princ, SP_PRINCESS_0_W, SP_PRINCESS_0_H);

   while (1);
}

void main(void) {
   init();
   game();
}
