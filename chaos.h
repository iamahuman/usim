#ifndef USIM_CHAOS_H
#define USIM_CHAOS_H

#include <stdio.h>
#include <pthread.h>

extern int chaos_init(void);
extern int chaos_poll(void);
extern void chaos_reconnect(void);

extern int chaos_get_addr(void);
extern int chaos_get_csr(void);
extern void chaos_set_csr(int v);
extern int chaos_get_rcv_buffer(void);
extern int chaos_get_bit_count(void);

extern void chaos_put_xmit_buffer(int v);
extern void chaos_xmit_pkt(void);

#endif
