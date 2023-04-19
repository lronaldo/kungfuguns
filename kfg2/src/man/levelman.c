#include "levelman.h"
#include <man/entityman.h>
#include <sys/behavioursys.h>
#include <assets/princess.h>
#include <assets/agent1.h>
#include <cpctelera.h>
#include <utils.h>

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// DATA
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
Entity_t const player_tmpl = {
      .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_INPUT_MASK | EM_CMP_PHYSICS_MASK)
   ,  .x = 10, .y = 10
   ,  .vx = 1, .vy = 0
   ,  .w = SP_PRINCESS_0_W
   ,  .h = SP_PRINCESS_0_H
   ,  .sprite = sp_princess_0
   ,  .beh = nullptr
   ,  .behdata = { 0, 0, 0, 0 }
};
Entity_t const enemy_tmpl = {
      .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_BEH_MASK | EM_CMP_PHYSICS_MASK)
   ,  .x = 70, .y = 10
   ,  .vx = -1, .vy = 0
   ,  .w = SP_ENEMY_CHINA_0_W
   ,  .h = SP_ENEMY_CHINA_0_H
   ,  .sprite = sp_enemy_china_0
   ,  .beh = sys_beh_bmove_to
   ,  .behdata = { 10, 50, 0, 0 }
};

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
static void 
start_level_1(void) {
   man_entity_create(&player_tmpl);
   man_entity_create(&enemy_tmpl);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void man_level_init(u8 const n) __z88dk_fastcall {
   n;
   man_entity_init();
   start_level_1();
}
