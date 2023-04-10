#include <cpctelera.h>
#include <assets/g_palette.h>
#include <assets/princess.h>

#define SCR_PTR_U16(VMEM,X,Y) (u16)((VMEM) + 80 * ((u16)((Y) >> 3)) + 2048 * ((Y) & 7) + (X))

void init() {
   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setPalette(g_palette, 16);
   cpct_setBorder(HW_LIME);
}

void game() {
   enum {
      p_string = SCR_PTR_U16(0xC000,  0,  96),
      p_princ  = SCR_PTR_U16(0xC000, 38, 140)
   };
   cpct_setDrawCharM0(1, 0);
   cpct_drawStringM0("Welcome to CPCtelera!", (void*)p_string);
   cpct_drawSprite(sp_princess_0, (void*)p_princ, SP_PRINCESS_0_W, SP_PRINCESS_0_H);

   while (1);
}

void main(void) {
   init();
   game();
}
