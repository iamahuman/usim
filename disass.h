#ifndef USIM_DISASS_H
#define USIM_DISASS_H

#include "ucode.h"

extern void disassemble_ucode_loc(ucw_t u);
extern void disassemble_instruction(uint32_t fefptr, uint32_t loc, int even, uint32_t inst, uint32_t width);

#endif
