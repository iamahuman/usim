#ifndef USIM_SYMS_H
#define USIM_SYMS_H

struct sym_s {
	struct sym_s *next;
	char *name;
	unsigned int v;
	int mtype;
};

struct symtab_s {
	char *name;
	struct sym_s *syms;
	int sym_count;
	struct sym_s **sorted_syms;
};

extern void read_sym_file(int mcr, char *fn);

extern int sym_find(int mcr, char *name, int *pval);
extern char *sym_find_by_val(int mcr, int v);
extern char *sym_find_by_type_val(int mcr, int t, int v);
extern char *sym_find_last(int mcr, int v, int *poffset);

#endif
