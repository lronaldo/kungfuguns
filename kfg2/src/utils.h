#pragma once

// Compile time constants
enum {
      NULL     = 0x0000
   ,  false    = 0
   ,  true     = 1
};

// Nullptr must be a define
#define nullptr (void*)NULL

// Screen pointer calculation, compile time version (for enums)
#define SCR_PTR_U16(VMEM,X,Y) (u16)((VMEM) + 80 * ((u16)((Y) >> 3)) + 2048 * ((Y) & 7) + (X))