#ifndef USIM_DISASS_H
#define USIM_DISASS_H

#include "syms.h"

extern char *uinst_desc(uint64_t u, symtab_t *symtab);
extern void disassemble_instruction(uint32_t fefptr, uint32_t loc, int even, uint32_t inst, uint32_t width);

#endif
