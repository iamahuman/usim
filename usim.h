#ifndef USIM_USIM_H
#define USIM_USIM_H

#include <stdbool.h>

#define tracef(args...)
#define tracevm(args...)
#define traceint(args...)
#define tracedio(args...)
#define tracenet(args...)
#define traceio(args...)

extern char *mcrsym_filename;

extern bool save_state_flag;
extern bool warm_boot_flag;
extern bool run_ucode_flag;

#endif
