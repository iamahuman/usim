#ifndef USIM_KBD_H
#define USIM_KBD_H

#include <stdint.h>

extern uint32_t kbd_key_scan;

extern unsigned short kbd_translate_table[3][256];

extern void kbd_init(void);
extern void kbd_warm_boot_key(void);
extern void kbd_key_event(int code, int keydown);
extern void kbd_dequeue_key_event(void);

#endif
