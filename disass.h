#ifndef USIM_DISASS_H
#define USIM_DISASS_H

#include "ucode.h"

extern void disassemble_ucode_loc(ucw_t u);
extern void disassemble_instruction(unsigned int fefptr, unsigned int loc, int even, unsigned int inst, unsigned int width);
extern void showstr(int a);
extern void show_fef_func_name(unsigned int fefptr, unsigned int width);

//---!! User defined function.
extern unsigned int read_virt(int addr);

#endif
