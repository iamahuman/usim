#ifndef USIM_SPY_H
#define USIM_SPY_H

extern void spy_init(char *port);

extern void spy_unibus_read(int offset, unsigned int *pv);
extern void spy_unibus_write(int offset, unsigned int v);

#endif
