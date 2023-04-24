#include "behavioursys.h"
#include <cpctelera.h>
#include <man/entityman.h>
#include <man/levelman.h>
#include <utils.h>

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// DATA
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

///< Behaviour compile-time constants
enum {
      BDATA_TargetX = 0  // Behdata Byte 0 used for TargetX value (Move-To)
   ,  BDATA_TargetY = 1  // Behdata Byte 1 used for TargetY value (Move-To)
   ,  BDATA_ArrRange = 2 // Behdata Byte 2 used for Arrival Range value (Move-To)
};

///< Macro for accessing Behaviour Data
#define BEHDATA(e, val) ((e)->behdata[BDATA_##val])

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
bool
sys_beh_bnull(Entity_t * const e) { e; return false; }

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
i8 clamp(i8 const v, i8 const min, i8 const max) {
   if (v < min) return min; 
   if (v > max) return max;
   return v;
}

bool inrange(i8 const v, i8 const min, i8 const max) {
   return v >= min && v <= max;
}

bool
sys_beh_bmove_to(Entity_t * const e) {
   i8 const dx = BEHDATA(e, TargetX) - e->x;
   i8 const dy = BEHDATA(e, TargetY) - e->y;
   i8 const range = BEHDATA(e, ArrRange);

   e->vx = 0;
   if ( ! inrange(dx, -range, range) ) {
      e->vx = clamp(dx, -1, 1);
   } 
   e->vy = 0;
   if ( ! inrange(dy, -range, range) ) {
      e->vy = clamp(dy, -1, 1);
   } 

   return (e->vx == 0 && e->vy == 0);
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
static bool
setup_beh_move_to(Entity_t * const e, u8 const x, u8 const y) {
   e->behdata[BDATA_TargetX] = x;
   e->behdata[BDATA_TargetY] = y;
   e->beh = sys_beh_bmove_to;

   return false;
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
bool
sys_beh_benemy_basic(Entity_t * const e) {
   Entity_t* const p = man_level_getPlayer();
   setup_beh_move_to(e, p->x, p->y);

   return false;
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
static void
execute_behaviours(Entity_t * const e) { 
   e->behticks -= 1;
   if ( e->behticks == 0 ) {
      if ( e->beh(e) ) {
         e->behfbk(e);  // Fallback executed when behaviour finishes
      }
      e->behticks = e->behcycles;
   }
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
void
sys_beh_update(void) {
   u8 const valid_entities 
      = (u8)(EM_CMP_ALIVE_MASK | EM_CMP_BEH_MASK);

   man_entity_foreach(
         execute_behaviours
      ,  valid_entities
   );
}