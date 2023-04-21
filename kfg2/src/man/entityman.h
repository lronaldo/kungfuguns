#pragma once
#include <types.h>
#include <utils.h>

///===========================================================================
///============================================================================
/// DATA STRUCTURES, CONSTANTS AND DECLARATIONS
///============================================================================
///============================================================================

/// Forward declarations
struct Entity_t;

///< Pointer to entity processing function
typedef void(*EntityFunPtr)(struct Entity_t * const);

///< Pointer to entity processing function
typedef bool(*BehaviourFunPtr)(struct Entity_t * const);

///< Entity Structure
typedef struct Entity_t {
   u8 cmps;                // Components
   u8 x, y;                // Screen coords
   u8 w, h;                // Sprite Size
   i8 vx, vy;              // Movement velocity
   void const* sprite;     // Sprite
   BehaviourFunPtr beh;    // Pointer to a behaviour function
   BehaviourFunPtr behfbk; // Behaviour fallback function (return after current behaviour finishes)
   u8 behdata[4];          // Behaviour data (bytes for different uses depending on behaviours)
} Entity_t;

///< Compile-time constants
enum {
     EM_MaxEntities     = 10     // Maximum number of entities
   , EM_FREE_ENTITY     = 0x00   // CMPS=0x00, Free entity (no components)
   , EM_DEFAULT_CMPS    = 0xFF   // CMPS=0xFF, Default components
   , EM_CMP_ALIVE_MASK  = 0x01   // Mask for Component that signals if an entity is alive or not
   , EM_CMP_INPUT_MASK  = 0x02   // Mask for Input Component
   , EM_CMP_PHYSICS_MASK= 0x04   // Mask for Physics Component
   , EM_CMP_BEH_MASK    = 0x08   // Mask for Behaviour Component
};

///< Check if an entity (pointer) has some components 
#define HAS_COMPONENTS_PTR(e,cs) (((e)->cmps & (cs)) == (cs))

///< Check if an entity is a valid, which means it is in use, so it is not free
#define IS_VALID_ENTITY(e) ((e)->cmps != EM_FREE_ENTITY)

///============================================================================
///============================================================================
/// PUBLIC FUNCTIONS
///============================================================================
///============================================================================
void      man_entity_init(void);
Entity_t* man_entity_create(Entity_t const * const e_template) __z88dk_fastcall;
Entity_t* man_entity_getByID(u8 const id) __z88dk_fastcall;
void      man_entity_forall(EntityFunPtr const Process) __z88dk_fastcall;
void      man_entity_foreach(EntityFunPtr const Process, u8 const cmps);
