#ifndef USIM_USIM_H
#define USIM_USIM_H

// Supported system verisons:
#define LISPM_SYSTEM78 7800L
#define LISPM_SYSTEM99 9900L

#define LISPM_SYSTEM LISPM_SYSTEM78

#include <stdbool.h>

#include "syms.h"

extern symtab_t sym_mcr;
extern symtab_t sym_prom;

extern bool warm_boot_flag;

#endif
