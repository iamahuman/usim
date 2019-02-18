#ifndef USIM_SYMS_H
#define USIM_SYMS_H

extern int sym_read_file(int mcr, char *fn);
extern char *sym_find_by_type_val(int mcr, int t, int v);
extern int sym_find(int mcr, char *name, int *pval);

#endif
