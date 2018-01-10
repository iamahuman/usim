#ifndef USIM_ETHER_H
#define USIM_ETHER_H

extern int ether_init(void);
extern void ether_poll(void);

extern void ether_xbus_reg_read(int offset, unsigned int *pv);
extern void ether_xbus_reg_write(int offset, unsigned int v);
extern void ether_xbus_desc_read(int offset, unsigned int *pv);
extern void ether_xbus_desc_write(int offset, unsigned int v);

#endif
