// syms.c --- routines for handling CADRLP symbol tables

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#include <sys/queue.h>

#include "usim.h"
#include "syms.h"

static void
sym_add(symtab_t *tab, int memory, char *name, uint32_t v)
{
	sym_t *s;

	s = (sym_t *) malloc(sizeof(sym_t));
	if (s == NULL) {
		perror("malloc");
		exit(1);
	}

	s->name = strdup(name);
	s->v = v;
	s->mtype = memory;

	tab->sym_count++;

	LIST_INSERT_HEAD(&tab->syms, s, next);
}

char *
sym_find_by_type_val(symtab_t *tab, symtype_t memory, uint32_t v)
{
	sym_t *s;

	LIST_FOREACH(s, &tab->syms, next) {
		if (s->v == v && s->mtype == memory)
			return s->name;
	}

	return 0;
}

int
sym_find(symtab_t *tab, char *name, int *pval)
{
	sym_t *s;

	LIST_FOREACH(s, &tab->syms, next) {
		if (strcasecmp(name, s->name) == 0) {
			*pval = s->v;
			return 0;
		}
	}

	return -1;
}

// Read a CADR MCR symbol file.
int
sym_read_file(symtab_t *tab, char *filename)
{
	FILE *f;
	char line[8 * 1024];

	f = fopen(filename, "r");
	if (f == NULL)
		return -1;

	LIST_INIT(&tab->syms);

	tab->name = strdup(filename);

	fgets(line, sizeof(line), f);
	fgets(line, sizeof(line), f);
	fgets(line, sizeof(line), f);

	while (fgets(line, sizeof(line), f) != NULL) {
		char sym[64];
		char symtype[64];
		int loc;
		int n;

		n = sscanf(line, "%s %s %o", sym, symtype, &loc);
		if (n == 3) {
			int type = 0;

			if (strcmp(symtype, "I-MEM") == 0)
				type = 1;
			else if (strcmp(symtype, "D-MEM") == 0)
				type = 2;
			else if (strcmp(symtype, "A-MEM") == 0)
				type = 4;
			else if (strcmp(symtype, "M-MEM") == 0)
				type = 5;
			else if (strcmp(symtype, "NUMBER") == 0)
				type = 6;
			else
				printf("? %s", symtype);

			sym_add(tab, type, sym, loc);
		}
	}

	fclose(f);

	return 0;
}
