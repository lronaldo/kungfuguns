#include "physicssys.h"
#include <man/entityman.h>
#include <cpctelera.h>
#include <utils.h>

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// DATA
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void
move_one_entity(Entity_t * const e) {
   e->x += e->vx;
   e->y += e->vy;
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
void
sys_physics_update(void) {
   enum { valid_entities = EM_CMP_ALIVE_MASK | EM_CMP_PHYSICS_MASK };

   man_entity_foreach(move_one_entity, valid_entities);
}