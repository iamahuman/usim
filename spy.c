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
spy_init(char *port)
{
	int err;
	struct termios oldtio;
	struct termios newtio;

	fd = open(port, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror(port);
		exit (1);
	}
}

void
spy_unibus_read(int offset, unsigned int *pv)
{
}

void
spy_unibus_write(int offset, unsigned int v)
{
}
