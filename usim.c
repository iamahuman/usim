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
#include "ucfg.h"
#include "utrace.h"
#include "ucode.h"
#include "mem.h"
#include "iob.h"
#include "tv.h"
#include "kbd.h"
#include "chaos.h"
#include "disk.h"

#include "syms.h"
#include "disass.h"

static char *config_filename;
bool warm_boot_flag = false;

symtab_t sym_mcr;
symtab_t sym_prom;

static void
sigusr1_handler(int arg)
{
	save_state(ucfg.usim_state_filename);
}

static void
usage(void)
{
	fprintf(stderr, "usage: usim [OPTION]...\n");
	fprintf(stderr, "CADR simulator\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -c FILE        configuration file (default: %s)\n", config_filename);
	fprintf(stderr, "  -w             warm boot\n");
	fprintf(stderr, "  -h             help message\n");
}

int
main(int argc, char *argv[])
{
	int c;

	printf("CADR emulator v" VERSION "\n");

	config_filename = "usim.ini";
	warm_boot_flag = false;

	while ((c = getopt(argc, argv, "c:wh")) != -1) {
		switch (c) {
		case 'c': config_filename = strdup(optarg); break;
		case 'w': warm_boot_flag = true; break;
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

	trace_stream = stderr;

	if (ini_parse(config_filename, ucfg_handler, &ucfg) < 0)
		fprintf(stderr, "Can't load '%s', using defaults\n", config_filename);

	signal(SIGUSR1, sigusr1_handler);

	read_prom(ucfg.ucode_prommcr_filename);
	sym_read_file(&sym_prom, ucfg.ucode_promsym_filename);

	tv_init();
	disk_init(ucfg.usim_disk_filename);
	sym_read_file(&sym_mcr, ucfg.ucode_mcrsym_filename);
	iob_init();
	chaos_init();

	if (warm_boot_flag == true) {
		kbd_warm_boot_key();
	}

	run();

	exit(0);
}
