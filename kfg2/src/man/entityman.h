#pragma once
#include <types.h>

typedef struct {
   u8 cmps;       // Components
} Entity_t;

enum {
     EM_MaxEntities  = 10      //
   , EM_FREE_ENTITY  = 0x00    // CMPS=0x00, Free entity (no components)
   , EM_DEFAULT_CMPS = 0xFF    // CMPS=0xFF, Default components
};

typedef void(*EntityFuncPtr)(Entity_t * const);


/////

void      man_entity_init();
Entity_t* man_entity_create();
void      man_entity_forall(EntityFuncPtr const Process) __z88dk_fastcall;