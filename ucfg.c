// ucfg.c --- configuration handling

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ini.h"

#include "ucfg.h"
#include "chaos.h"

#include "misc.h"

bool save_state_flag = false;
bool warm_boot_flag = false;

ucfg_t ucfg = {
#define X(s, n, default) default,
#include "ucfg.defs"
#undef X
};

#define INIHEQ(s, n) (streq(s, section) && streq(n, name)) 

int
ucfg_handler(void *user, const char *section, const char *name, const char *value)
{
	ucfg_t *cfg = (ucfg_t *) user;

	if (0) ;
#define X(s, n, default)						\
	else if (INIHEQ(#s, #n)) cfg->s##_##n = strdup(value);
#include "ucfg.defs"
#undef X

	if (INIHEQ("chaos", "myaddr")) {
		int addr;
		char *end;

		addr = strtoul(value, &end, 8);
		if (*end != 0 || addr > 0177777) {
			fprintf(stderr, "Chaosnet address must be a 16-bit octal number\n");
			exit(1);
		}
		chaos_set_addr(addr);
	}
	
	return 1;
}
