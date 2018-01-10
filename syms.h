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

extern int sym_find(int mcr, char *name, int *pval);
extern char *sym_find_by_val(int mcr, int v);
extern char *sym_find_by_type_val(int mcr, int t, int v);
extern char *sym_find_last(int mcr, int v, int *poffset);
extern int _sym_read_file(struct symtab_s *tab, const char *filename);
extern int _sym_sort(struct symtab_s *tab);

extern void read_promsym_file(void);
extern void read_mcrsym_file(void);

#endif
