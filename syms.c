// syms.c --- routines for handling CADRLP symbol tables

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#include <sys/queue.h>

#include "usim.h"

struct sym_s {
	LIST_ENTRY(sym_s) next;
	char *name;
	unsigned int v;
	int mtype;
};

struct symtab_s {
	char *name;
	int sym_count;
	LIST_HEAD(syms, sym_s) syms;
};

struct symtab_s sym_prom;
struct symtab_s sym_mcr;

static void
_sym_add(struct symtab_s *tab, int memory, char *name, unsigned int v)
{
	struct sym_s *s;

	s = (struct sym_s *) malloc(sizeof(struct sym_s));
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

static char *
_sym_find_by_val(struct symtab_s *tab, int memory, unsigned int v)
{
	struct sym_s *s;

	LIST_FOREACH(s, &tab->syms, next) {
		if (s->v == v && s->mtype == memory)
			return s->name;
	}

	return 0;
}

static int
_sym_find(struct symtab_s *tab, char *name, int *pval)
{
	struct sym_s *s;

	LIST_FOREACH(s, &tab->syms, next) {
		if (strcasecmp(name, s->name) == 0) {
			*pval = s->v;
			return 0;
		}
	}

	return -1;
}

// Read a CADR MCR symbol file.
static int
_sym_read_file(struct symtab_s *tab, const char *filename)
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

			_sym_add(tab, type, sym, loc);
		}
	}

	fclose(f);

	printf("[%s] %d symbols\n", tab->name, tab->sym_count);

	return 0;
}

int
sym_read_file(int mcr, char *fn)
{
	if (mcr)
		return _sym_read_file(&sym_mcr, fn);
	return _sym_read_file(&sym_prom, fn);
}

int
sym_find(int mcr, char *name, int *pval)
{
	if (mcr)
		return _sym_find(&sym_mcr, name, pval);
	return _sym_find(&sym_prom, name, pval);
}

char *
sym_find_by_type_val(int mcr, int t, int v)
{
	if (mcr)
		return _sym_find_by_val(&sym_mcr, t, v);
	return _sym_find_by_val(&sym_prom, t, v);
}
