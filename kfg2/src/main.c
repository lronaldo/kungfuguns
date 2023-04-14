#include <cpctelera.h>
#include <assets/g_palette.h>
#include <assets/princess.h>
#include "utils.h"
#include <sys/rendersys.h>
#include <sys/physicssys.h>
#include <man/entityman.h>

void init() {
   cpct_disableFirmware();
   cpct_setVideoMode(0);
   cpct_setPalette(g_palette, 16);
   cpct_setBorder(HW_LIME);
}

Entity_t const player_tmpl = {
   .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_INPUT_MASK | EM_CMP_PHYSICS_MASK),
   .x = 10, .y = 10,
   .vx = 1, .vy = 0,
   .w = SP_PRINCESS_0_W,
   .h = SP_PRINCESS_0_H,
   .sprite = sp_princess_0
};

void game() {
   man_entity_init();

   {  Entity_t* player = man_entity_create();
      cpct_memcpy(player, &player_tmpl, sizeof(Entity_t));
   }

   while (1) {
      sys_render_update();
      sys_physics_update();

      cpct_waitVSYNCStart();
   }
}

void main(void) {
   init();
   game();
}
