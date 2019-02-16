// usim --- MIT CADR simulator

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
	fprintf(stderr, "  -S             save state\n");
	fprintf(stderr, "  -s             halt after prom runs\n");
	fprintf(stderr, "  -w             warm boot\n");
	fprintf(stderr, "  -h             help message\n");
}

int
main(int argc, char *argv[])
{
	int c;

	printf("CADR emulator v0.9\n");

	while ((c = getopt(argc, argv, "i:Sswl::hA:")) != -1) {
		switch (c) {
		case 'i':
			disk_filename = strdup(optarg);
			break;
		case 'S':
			save_state_flag = true;
			break;
		case 's':
			stop_after_prom_flag = true;
			break;
		case 'w':
			warm_boot_flag = true;
			break;
		case 'A':
			if (optarg != NULL) {
				char *end;
				chaos_addr = (int)strtoul (optarg, &end, 8);
				if (*end != 0 || chaos_addr > 0177777) {
					fprintf (stderr, "Chaosnet address must be a 16-bit octal number\n");
					exit(1);
				}
			}
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

	if (argc > 0) {
		usage();
		exit(1);
	}

	read_prom_files();

	tv_init();
	disk_init(disk_filename);
	iob_init();
	chaos_init();

	if (warm_boot_flag == true)
		kbd_warm_boot_key();

	while (run())
		;

	exit(0);
}
