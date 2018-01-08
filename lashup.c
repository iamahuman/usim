#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int fd;

void
lashup_init(char *port)
{
	fd = open(port, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror(port);
		exit (1);
	}
}

void
lashup_unibus_read(int offset, unsigned int *pv)
{
	switch (offset) {
	case 0100:
		printf("lashup: reading from debugee\n");
		break;
	case 0114:
		printf("lashup: reading from write only address (debugee address)\n");
		break;
	case 0110:
		printf("lashup: reading from write only address (modifier bits)\n");
		break;
	case 0104:
		printf("lashup: bus cycle)\n");
		break;
	default:
		printf("lashup: read: unknown offset: %o\n", offset);
		break;
	}
}

void
lashup_unibus_write(int offset, unsigned int v)
{
	switch (offset) {
	case 0100:
		printf("lashup: writing to debugee\n");
		break;
	case 0114:
		printf("lashup: unibus debugee 1-16 bits\n");
		break;
	case 0110: {
		int bits = v;
		printf("lashup: writing modifier bits\n");
		switch (bits) {
		case 01:
			printf("\tbit 17 of debugee unibus address\n");
			break;
		case 02:
			printf("\treset debugee bus interface\n");
			break;
		case 04:
			printf("\ttimeout inhibit\n");
			break;
		default:
			printf("\tunknown modifier bit: %o; v %o\n", bits, v);
			break;
		}
		break;
	}

	case 0104:
		printf("lashup: writing to read-only address (bus cycle)\n");
		break;
	default:
		printf("lashup: write: unknown offset: %o; v %o\n", offset, v);
		break;
	}
}
