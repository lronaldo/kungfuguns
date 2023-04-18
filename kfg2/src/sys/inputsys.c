#include "inputsys.h"
#include <cpctelera.h>
#include <man/entityman.h>
#include <utils.h>

// Forward function declarations
static void act_left(Entity_t * const);
static void act_right(Entity_t * const);

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// DATA
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

///< Input Systems' compile-time constants
static enum {
   INPUT_ENTITY = 0        ///< Only entity 0 has input
};

///< A pair (Key, Action) for actions on keypresses
typedef struct { 
   cpct_keyID key;                  ///< Key to be pressed
   void(*action)(Entity_t * const); ///< Action to be performed on keypress
} KeyAction_t;

///< Possible keys the user can press and their associated actions
KeyAction_t const keytable[] = {
      { Key_O  , act_left  }
   ,  { Key_P  , act_right }
   ,  { nullptr, (void*)nullptr }
};

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
/// FUNCTIONS
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void act_left (Entity_t * const e) { e->vx = -1; }
static void act_right(Entity_t * const e) { e->vx = +1; }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void
check_user_input(Entity_t * const player) {
   KeyAction_t const * k = keytable;

   cpct_scanKeyboard_f();
   if ( cpct_isAnyKeyPressed() == false ) return;

   while( k->key != nullptr ) {
      if ( cpct_isKeyPressed(k->key) )
         k->action(player);
      ++k;
   }
}

/////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
void
sys_input_update(void) {
   u8 const cmps = (u8)(EM_CMP_ALIVE_MASK | EM_CMP_INPUT_MASK);
   Entity_t* player = man_entity_getByID(INPUT_ENTITY);
   
   if ( HAS_COMPONENTS_PTR(player, cmps) ) {
      player->vx = 0;
      player->vy = 0;
      check_user_input(player);
   }
}