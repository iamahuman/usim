#ifndef USIM_UTRACE_H
#define USIM_UTRACE_H

#define TRACE_MISC	0001 // 0000_0001
#define TRACE_VM	0002 // 0000_0010
#define TRACE_INT	0004 // 0000_1000
#define TRACE_DISK	0010 // 0001_0000
#define TRACE_CHAOS	0020 // 0100_0000
#define TRACE_IOB	0040 // 1000_0000

#define TRACE_ALL 	0377 // 1111_1111

#include "trace.h"

#endif
