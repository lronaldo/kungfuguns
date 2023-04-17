#include "levelman.h"
#include <man/entityman.h>
#include <assets/princess.h>
#include <assets/agent1.h>
#include <cpctelera.h>

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// DATA
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
Entity_t const player_tmpl = {
   .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_INPUT_MASK | EM_CMP_PHYSICS_MASK),
   .x = 10, .y = 10,
   .vx = 1, .vy = 0,
   .w = SP_PRINCESS_0_W,
   .h = SP_PRINCESS_0_H,
   .sprite = sp_princess_0
};
Entity_t const enemy_tmpl = {
   .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_AI_MASK | EM_CMP_PHYSICS_MASK),
   .x = 70, .y = 10,
   .vx = -1, .vy = 0,
   .w = SP_ENEMY_CHINA_0_W,
   .h = SP_ENEMY_CHINA_0_H,
   .sprite = sp_enemy_china_0
};

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
static void 
start_level_1(void) {
   Entity_t* e = man_entity_create();
   cpct_memcpy(e, &player_tmpl, sizeof(Entity_t));
   e = man_entity_create();
   cpct_memcpy(e, &enemy_tmpl, sizeof(Entity_t));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void man_level_init(u8 const n) __z88dk_fastcall {
   n;
   man_entity_init();
   start_level_1();
}
