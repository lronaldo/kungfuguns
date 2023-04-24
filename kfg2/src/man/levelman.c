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
   ,  .w       = SP_PRINCESS_0_W
   ,  .h       = SP_PRINCESS_0_H
   ,  .sprite  = sp_princess_0
   ,  .beh     = nullptr
   ,  .behfbk  = nullptr
   ,  .behcycles = 0
   ,  .behticks = 0
   ,  .behdata = { 0, 0, 0, 0 }
};
Entity_t const enemy_tmpl = {
      .cmps = (EM_CMP_ALIVE_MASK | EM_CMP_BEH_MASK | EM_CMP_PHYSICS_MASK)
   ,  .x = 70, .y = 10
   ,  .vx = -1, .vy = 0
   ,  .w       = SP_ENEMY_CHINA_0_W
   ,  .h       = SP_ENEMY_CHINA_0_H
   ,  .sprite  = sp_enemy_china_0
   ,  .beh     = sys_beh_benemy_basic
   ,  .behfbk  = sys_beh_benemy_basic
   ,  .behcycles = 5
   ,  .behticks = 1
   ,  .behdata = { 10, 50, 2, 0 }
};
Entity_t* player_;

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void 
start_level_1(void) {
   player_ = man_entity_create(&player_tmpl);
   man_entity_create(&enemy_tmpl);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Entity_t * const man_level_getPlayer() { return player_; }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void man_level_init(u8 const n) __z88dk_fastcall {
   n;
   man_entity_init();
   start_level_1();
}
