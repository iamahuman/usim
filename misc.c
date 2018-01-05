#include <stdio.h>
#include <stdlib.h>

unsigned int
read16(int fd)
{
	unsigned char b[2];
	int ret;

	ret = read(fd, b, 2);
	if (ret < 2) {
		printf("eof!\n");
		exit(1);
	}
	return (b[1] << 8) | b[0];
}

unsigned int
read32(int fd)
{
	unsigned char b[4];
	int ret;

	ret = read(fd, b, 4);
	if (ret < 4) {
		printf("eof!\n");
		exit(1);
	}
	return (b[1] << 24) | (b[0] << 16) | (b[3] << 8) | b[2];
}

