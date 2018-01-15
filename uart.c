// uart.c --- ---!!!

#include <stdio.h>

int
uart_init(void)
{
	return 0;
}

void
uart_poll(void)
{
}

void
uart_xbus_read(int offset, unsigned int *pv)
{
	*pv = 0;
}

void
uart_xbus_write(int offset, unsigned int v)
{
	if (offset == 0) {
		putc(v, stdout);
		fflush(stdout);
	}
}
