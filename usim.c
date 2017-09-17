#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>

#include "usim.h"
#include "ucode.h"
#include "iob.h"
#include "tv.h"
#include "kbd.h"
#include "chaos.h"
#include "Files.h"
#include "disk.h"

#include "syms.h"
#include "decode.h"

char *disk_filename = "disk.img";
char *mcrsym_filename = "../bands/ucadr.sym.841";
char *promsym_filename = "../bands/promh.sym.9";

bool run_ucode_flag = true;
bool save_state_flag = false;
bool warm_boot_flag = false;
bool stop_after_prom_flag = false;
bool prom_enabled_flag = true;

void
usage(void)
{
	fprintf(stderr, "usage: usim [OPTION]...\n");
	fprintf(stderr, "CADR simulator\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -i FILE        set disk image\n");
	fprintf(stderr, "  -r             map /tree to ../sys\n");
	fprintf(stderr, "  -S             save state\n");
	fprintf(stderr, "  -s             halt after prom runs\n");
	fprintf(stderr, "  -w             warm boot\n");
	exit(1);
}

extern char *optarg;

int
main(int argc, char *argv[])
{
	int c;

	printf("CADR emulator v0.9\n");

	while ((c = getopt(argc, argv, "ab:B:c:dC:i:l:nmp:q:rtT:sSw")) != -1) {
		switch (c) {
		case 'i':
			disk_filename = strdup(optarg);
			break;
		case 'r':
		{
			char *p = "../sys";
			char newpath[PATH_MAX];

			realpath(p, newpath);
			dcanon(newpath, 0);
			settreeroot(newpath);
		}
		break;
		case 'S':
			save_state_flag = 1;
			break;
		case 's':
			stop_after_prom_flag = 1;
			break;
		case 'w':
			warm_boot_flag = 1;
			break;
		default:
			usage();
		}
	}

	tv_init();
	tv_poll();

	disk_init(disk_filename);

	read_prom_files();
	read_sym_files();

	iob_init();
	chaos_init();

	if (warm_boot_flag) {
		kbd_warm_boot_key();
	}

	while (run()) {
		tv_poll();
	}

	exit(0);
}
