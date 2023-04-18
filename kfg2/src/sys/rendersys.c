#include <man/entityman.h>
#include <cpctelera.h>

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
sys_render_one_entity(Entity_t const* e) {
   u8* const pvideo = cpct_getScreenPtr((void*)0xC000, e->x, e->y);
   cpct_drawSprite((void*)e->sprite, pvideo, e->w, e->h);
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
void
sys_render_update(void) {
   man_entity_forall(sys_render_one_entity);
}