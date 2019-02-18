#ifndef USIM_DISASS_H
#define USIM_DISASS_H

#include "ucode.h"

extern void disassemble_ucode_loc(ucw_t u);
extern void disassemble_instruction(uint32_t fefptr, uint32_t loc, int even, uint32_t inst, uint32_t width);
extern void showstr(int a);
extern void show_fef_func_name(uint32_t fefptr, uint32_t width);

//---!! User defined function.
extern uint32_t read_virt(int addr);

#endif
