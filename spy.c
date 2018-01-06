#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "usim.h"

int fd;

void
spy_init(char *port)
{
	fd = open(port, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror(port);
		exit (1);
	}
}

void
spy_unibus_read(int offset, unsigned int *pv)
{
	switch (offset) {
	case 000:
		traceio("unibus: read IR<15-0>\n");
		*pv = 0;
		break;
	case 002:
		traceio("unibus: read IR<31-16>\n");
		*pv = 0;
		break;
	case 004:
		traceio("unibus: read IR<47-32>\n");
		*pv = 0;
		break;
	case 006: // Not used.
		*pv = 0;
		break;
	case 010:
		traceio("unibus: read OPC\n");
		*pv = 0;
		break;
	case 012:
		traceio("unibus: read PC\n");
		*pv = 0;
		break;
	case 014:
		traceio("unibus: read OB<15-0>\n");
		*pv = 0;
		break;
	case 016:
		traceio("unibus: read OB<31-16>\n");
		*pv = 0;
		break;
	case 020:
		traceio("unibus: read Flag Register 1\n");
		*pv = 0;
		break;
	case 022:
		traceio("unibus: read Flag Register 2\n");
		*pv = 0;
		break;
	case 024:
		traceio("unibus: read M<15-0>\n");
		*pv = 0;
		break;
	case 026:
		traceio("unibus: read M<31-16>\n");
		*pv = 0;
		break;
	case 030:
		traceio("unibus: read A<15-0>\n");
		*pv = 0;
		break;
	case 032:
		traceio("unibus: read A<31-16>\n");
		*pv = 0;
		break;
	case 034:
		traceio("unibus: read ST<15-0>\n");
		*pv = 0;
		break;
	case 036:
		traceio("unibus: read ST<31-16>\n");
		*pv = 0;
		break;
	}
}

void
spy_unibus_write(int offset, unsigned int v)
{
	switch (offset) {
	case 000:
		traceio("unibus: write DEBUG-IR<15-0> %o\n",v);
		break;
	case 002:
		traceio("unibus: write DEBUG-IR<31-16>\n", v);
		break;
	case 004:
		traceio("unibus: write DEBUG-IR<47-32>\n", v);
		break;
	case 006:
		traceio("unibus: write clock control register.\n", v);
		break;
	case 010:
		traceio("unibus: write OPC control register\n", v);
		break;
	//---!!! 012: Handled directly in unibus_write.
	case 014: // Not used.
	case 016: // Not used.
		break;
	}
}
