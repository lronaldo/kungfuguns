#pragma once
#include <types.h>

///===========================================================================
///============================================================================
/// DATA STRUCTURES, CONSTANTS AND DECLARATIONS
///============================================================================
///============================================================================

/// @brief Entity Structure
typedef struct {
   u8 cmps;             // Components
   u8 x, y;             // Screen coords
   u8 w, h;             // Sprite Size
   i8 vx, vy;           // Movement velocity
   void const* sprite;  // Sprite
} Entity_t;

/// @brief Compile-time constants
enum {
     EM_MaxEntities     = 10     // Maximum number of entities
   , EM_FREE_ENTITY     = 0x00   // CMPS=0x00, Free entity (no components)
   , EM_DEFAULT_CMPS    = 0xFF   // CMPS=0xFF, Default components
   , EM_CMP_ALIVE_MASK  = 0x01   // Mask for Component that signals if an entity is alive or not
   , EM_CMP_INPUT_MASK  = 0x02   // Mask for Input Component
   , EM_CMP_PHYSICS_MASK= 0x04   // Mask for Physics Component
   , EM_CMP_AI_MASK     = 0x08   // Mask for AI Component
};

/// @brief Pointer to entity processing function
typedef void(*EntityFuncPtr)(Entity_t * const);

///============================================================================
///============================================================================
/// PUBLIC FUNCTIONS
///============================================================================
///============================================================================
void      man_entity_init(void);
Entity_t* man_entity_create(void);
void      man_entity_forall(EntityFuncPtr const Process) __z88dk_fastcall;
void      man_entity_foreach(EntityFuncPtr const Process, u8 const cmps);
