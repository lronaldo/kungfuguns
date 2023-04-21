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
      BDATA_TargetX = 0 // Behdata Byte 0 used for TargetX value (Move-To)
   ,  BDATA_TargetY = 1 // Behdata Byte 1 used for TargetY value (Move-To)
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
bool
sys_beh_bmove_to(Entity_t * const e) {
   i8 VX = e->x - BEHDATA(e, TargetX);
   i8 VY = e->y - BEHDATA(e, TargetY);
   if (VX < 0) VX = 1; else if (VX > 0) VX = -1;
   if (VY < 0) VY = 1; else if (VY > 0) VY = -1;
   e->vx = VX;
   e->vy = VY;

   return (VX == 0 && VY == 0);
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
   *((u8*)0xC000) = 0;
   if ( e->beh(e) ) {
      *((u8*)0xC000) = 0xFF;
      e->behfbk(e);  // Fallback executed when behaviour finishes
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