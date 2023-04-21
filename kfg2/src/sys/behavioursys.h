#pragma once
#include <utils.h>

/// Forward Declarations
struct Entity_t;

///============================================================================
///============================================================================
/// PUBLIC FUNCTIONS
///============================================================================
///============================================================================
void sys_beh_update        (void);
bool sys_beh_bnull         (struct Entity_t * const e);
bool sys_beh_bmove_to      (struct Entity_t * const e);
bool sys_beh_benemy_basic  (struct Entity_t * const e);
