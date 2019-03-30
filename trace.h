#ifndef USIM_TRACE_H
#define USIM_TRACE_H

#include <syslog.h>

#define TRACE_NONE	0	// 0000_0000

#ifndef TRACE_ALL
#define TRACE_ALL 0
#endif

extern int trace_level;
extern int trace_facilities;

extern FILE *trace_stream;
extern int trace_fd;

extern void trace(int facility, int prio, const char *fmt, ...);

#define emerg(facility, args...)	trace(facility, LOG_EMERG, args)
#define alert(facility, args...)	trace(facility, LOG_ALERT, args)
#define crit(facility, args...)		trace(facility, LOG_CRIT, args)
#define err(facility, args...)		trace(facility, LOG_ERR, args)
#define warning(facility, args...)	trace(facility, LOG_WARNING, args)
#define notice(facility, args...)	trace(facility, LOG_NOTICE, args)
#define info(facility, args...)		trace(facility, LOG_INFO, args)
#define debug(facility, args...)	trace(facility, LOG_DEBUG, args)

#endif
