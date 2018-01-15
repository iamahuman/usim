#ifndef USIM_UART_H
#define USIM_UART_H

extern int uart_init(void);
extern void uart_poll(void);
extern void uart_xbus_read(int offset, unsigned int *pv);
extern void uart_xbus_write(int offset, unsigned int v);

#endif
