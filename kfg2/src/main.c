#include <cpctelera.h>
#include <assets/g_palette.h>
#include "utils.h"
#include <sys/rendersys.h>
#include <sys/physicssys.h>
#include <sys/inputsys.h>
#include <man/levelman.h>

void init() {
   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setPalette(g_palette, 16);
   cpct_setBorder(HW_LIME);
}


void game() {
   man_level_init(1);

   while (1) {
      sys_render_update();
      sys_input_update();
      sys_physics_update();

      cpct_waitVSYNCStart();
   }
}

void main(void) {
   init();
   game();
}
