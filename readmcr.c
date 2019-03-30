// readmcr --- dump a microcode (MCR) file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#include "usim.h"
#include "ucode.h"
#include "disass.h"
#include "syms.h"
#include "misc.h"

static bool showimem = false;
static bool showamem = false;
static bool showdmem = false;
static char *symfn = NULL;

static char *
getlbl(int type, int loc)
{
	char *lbl;

	if (symfn == NULL)
		return "";

	lbl = sym_find_by_type_val(0, type, loc);
	if (lbl == NULL)
		return "";

	return lbl;
}

static void
dump_i_mem(int fd, int start, int size)
{
	int loc;

	printf("i-memory; start %o, size %o\n", start, size);
	loc = start;
	for (int i = 0; i < size; i++) {
		uint16_t w1;
		uint16_t w2;
		uint16_t w3;
		uint16_t w4;
		ucw_t ll;

		w1 = read16(fd);
		w2 = read16(fd);
		w3 = read16(fd);
		w4 = read16(fd);
		ll =
			((ucw_t) w1 << 48) |
			((ucw_t) w2 << 32) |
			((ucw_t) w3 << 16) |
			((ucw_t) w4 << 0);

		if (showimem) {
			char *l = getlbl(1, loc);
			if (strcmp(l, "") != 0)
				printf("%s:\n", l);

			printf("%03o %016" PRIo64 ":\t", loc, ll);
			disassemble_ucode_loc(ll);
		}

		loc++;
	}
}

static void
dump_d_mem(int fd, int start, int size)
{
	printf("d-memory; start %o, size %o\n", start, size);

	if (size < 04000)
		fprintf(stderr, "WARNING: D-MEM is less than 04000 words\n");

	for (int i = 0; i < size; i++) {
		unsigned int v;

		v = read32(fd);

		if (showdmem == true)
			printf("%o <- %o\t%s\n", i, v, getlbl(2, i));
	}
}

static void
dump_main_mem(int fd, int start, int size)
{
	printf("main-memory; start %d, size %d\n", start, size);
	read32(fd);
	lseek(fd, 0, SEEK_CUR);
}

static void
dump_a_mem(int fd, int start, int size)
{
	printf("a-memory; start %o, size %o\n", start, size);
	for (int i = 0; i < size; i++) {
		unsigned int v;

		v = read32(fd);

		if (showamem == true)
			printf("%o <- %o\t%s %s\n", i, v, getlbl(4, i), getlbl(5, i));
	}
}

static void
usage(void)
{
	fprintf(stderr, "usage: readmcr FILE\n");
	fprintf(stderr, "dump a microcode file\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -i             show I memory (microcode) section\n");
	fprintf(stderr, "  -a             show A memory section\n");
	fprintf(stderr, "  -d             show D memory section\n");
	fprintf(stderr, "  -s FILE	  decode labels from a symbol file\n");
	fprintf(stderr, "  -h             show help message\n");
}

int
main(int argc, char *argv[])
{
	int c;
	int fd;
	bool done;

	showimem = false;
	showamem = false;
	showdmem = false;
	symfn = NULL;

	while ((c = getopt(argc, argv, "iads:h")) != -1) {
		switch (c) {
		case 'i':
			showimem = true;
			break;
		case 'a':
			showamem = true;
			break;
		case 'd':
			showdmem = true;
			break;
		case 's':
			symfn = optarg;
			sym_read_file(0, symfn);
			break;
		case 'h':
			usage();
			exit(0);
		default:
			usage();
			exit(1);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage();
		exit(1);
	}

	fd = open(argv[0], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "%s: no such file or directory\n", argv[0]);
		exit(1);
	}

	done = false;
	while (!done) {
		int code;
		int start;
		int size;

		code = read32(fd);
		start = read32(fd);
		size = read32(fd);

		switch (code) {
		case 1:
			dump_i_mem(fd, start, size);
			break;
		case 2:
			dump_d_mem(fd, start, size);
			break;
		case 3:
			dump_main_mem(fd, start, size);
			break;
		case 4:
			dump_a_mem(fd, start, size);
			done = true;
			break;
		}
	}

	exit(0);
}
