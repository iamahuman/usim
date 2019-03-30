#ifndef USIM_USIM_H
#define USIM_USIM_H

#include <stdbool.h>

#include "syms.h"

#define tracef(args...)
#define tracevm(args...)
#define traceint(args...)
#define tracedio(args...)
#define tracenet(args...)
#define traceio(args...)

extern symtab_t sym_mcr;
extern symtab_t sym_prom;

extern bool warm_boot_flag;

#endif
