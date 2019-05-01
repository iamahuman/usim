// kbd.c --- keyboard handling

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>

#include "usim.h"
#include "ucfg.h"
#include "utrace.h"
#include "ucode.h"
#include "iob.h"
#include "kbd.h"

uint32_t kbd_key_scan;

#define KEY_QUEUE_LEN 10

static int key_queue[KEY_QUEUE_LEN];
static int key_queue_optr = 0;
static int key_queue_iptr = 0;
static int key_queue_free = KEY_QUEUE_LEN;

static void
queue_key_event(int ev)
{
	int v;

	v = (1 << 16) | ev;

	if (key_queue_free > 0) {
		DEBUG(TRACE_IOB, "queue_key_event() - queuing 0%o, q len before %d\n", v, KEY_QUEUE_LEN - key_queue_free);
		key_queue_free--;
		key_queue[key_queue_optr] = v;
		key_queue_optr = (key_queue_optr + 1) % KEY_QUEUE_LEN;
	} else {
		WARNING(TRACE_IOB, "IOB key queue full!");
		if (!(iob_csr & (1 << 5)) && (iob_csr & (1 << 2))) {
			iob_csr |= 1 << 5;
			WARNING(TRACE_IOB, "queue_key_event generating interrupt");
			assert_unibus_interrupt(0260);
		}
	}
}

void
kbd_dequeue_key_event(void)
{
	if (iob_csr & (1 << 5))	// Already something to be read.
		return;

	if (key_queue_free < KEY_QUEUE_LEN) {
		int v = key_queue[key_queue_iptr];
		DEBUG(TRACE_IOB, "dequeue_key_event() - dequeuing 0%o, q len before %d\n", v, KEY_QUEUE_LEN - key_queue_free);
		key_queue_iptr = (key_queue_iptr + 1) % KEY_QUEUE_LEN;
		key_queue_free++;
		kbd_key_scan = (1 << 16) | v;
		if (iob_csr & (1 << 2)) { // Keyboard interrupt enabled?
			iob_csr |= 1 << 5;
			WARNING(TRACE_IOB, "dequeue_key_event generating interrupt (q len after %d)", KEY_QUEUE_LEN - key_queue_free);
			assert_unibus_interrupt(0260);
		}
	}
}

void
kbd_key_event(int code, int keydown)
{
	int v;

	DEBUG(TRACE_IOB, "key_event(code=%x, keydown=%x)\n", code, keydown);

	v = ((!keydown) << 8) | code;

	if (iob_csr & (1 << 5))
		queue_key_event(v); // Already something there, queue this.
	else {
		kbd_key_scan = (1 << 16) | v;
		DEBUG(TRACE_IOB, "key_event() - 0%o\n", kbd_key_scan);
		if (iob_csr & (1 << 2)) {
			iob_csr |= 1 << 5;
			assert_unibus_interrupt(0260);
		}
	}
}

void
kbd_warm_boot_key(void)
{
	// Send a Return to get the machine booted.
	kbd_key_event(062, 0);
}

#include "knight.h"

// ASCII to Knight keyboard scancode translation table.
unsigned short kbd_translate_table[3][256];

static void
kbd_old_init(void)
{
	memset((char *) kbd_translate_table, 0, sizeof(kbd_translate_table));

	for (int i = 0; i < 64; i++) {
		unsigned char k;
		k = kbd_old_table[i][0];
		kbd_translate_table[0][(int) k] = i;
	}

	for (int i = 0; i < 255; i++)
		kbd_translate_table[1][i] = kbd_translate_table[0][i] | (3 << 6);

	// Modify mapping to match present-day US keyboard layout.
	kbd_translate_table[0]['`'] = 015 | (3 << 6);
	kbd_translate_table[1]['`'] = 016 | (3 << 6);

	kbd_translate_table[0]['\''] = 010 | (3 << 6);
	kbd_translate_table[1]['\''] = 3 | (3 << 6);
	kbd_translate_table[0]['='] = 014 | (3 << 6);
	kbd_translate_table[1]['2'] = 015;

	kbd_translate_table[1]['6'] = 016;
	kbd_translate_table[1]['7'] = 7 | (3 << 6);
	kbd_translate_table[1]['8'] = 061 | (3 << 6);
	kbd_translate_table[1]['9'] = 011 | (3 << 6);
	kbd_translate_table[1]['0'] = 012 | (3 << 6);
	kbd_translate_table[1]['-'] = 013 | (3 << 6);
	kbd_translate_table[1]['='] = 060 | (3 << 6);

	kbd_translate_table[1][';'] = 061;
	kbd_translate_table[1][':'] = 061;
	kbd_translate_table[1]['!'] = 2 | (3 << 6);
	kbd_translate_table[1]['"'] = 3 | (3 << 6);
	kbd_translate_table[1]['#'] = 4 | (3 << 6);
	kbd_translate_table[1]['$'] = 5 | (3 << 6);
	kbd_translate_table[1]['%'] = 6 | (3 << 6);
	kbd_translate_table[1]['&'] = 7 | (3 << 6);
	kbd_translate_table[1]['('] = 011 | (3 << 6);
	kbd_translate_table[1][')'] = 012 | (3 << 6);
	kbd_translate_table[1]['_'] = 013 | (3 << 6);
	kbd_translate_table[1]['~'] = 016 | (3 << 6);
	kbd_translate_table[1]['@'] = 13;
	kbd_translate_table[1]['^'] = 14;

	kbd_translate_table[1]['Q'] = 20 | (3 << 6);
	kbd_translate_table[1]['W'] = 21 | (3 << 6);
	kbd_translate_table[1]['E'] = 22 | (3 << 6);
	kbd_translate_table[1]['R'] = 23 | (3 << 6);
	kbd_translate_table[1]['T'] = 24 | (3 << 6);
	kbd_translate_table[1]['Y'] = 25 | (3 << 6);
	kbd_translate_table[1]['U'] = 26 | (3 << 6);
	kbd_translate_table[1]['I'] = 27 | (3 << 6);
	kbd_translate_table[1]['O'] = 28 | (3 << 6);
	kbd_translate_table[1]['P'] = 29 | (3 << 6);
	kbd_translate_table[1]['{'] = 30 | (3 << 6);
	kbd_translate_table[1]['}'] = 31 | (3 << 6);
	kbd_translate_table[1]['|'] = 32 | (3 << 6);

	kbd_translate_table[1]['A'] = 39 | (3 << 6);
	kbd_translate_table[1]['S'] = 40 | (3 << 6);
	kbd_translate_table[1]['D'] = 41 | (3 << 6);
	kbd_translate_table[1]['F'] = 42 | (3 << 6);
	kbd_translate_table[1]['G'] = 43 | (3 << 6);
	kbd_translate_table[1]['H'] = 44 | (3 << 6);
	kbd_translate_table[1]['J'] = 45 | (3 << 6);
	kbd_translate_table[1]['K'] = 46 | (3 << 6);
	kbd_translate_table[1]['L'] = 47 | (3 << 6);
	kbd_translate_table[1]['+'] = 48 | (3 << 6);
	kbd_translate_table[1]['*'] = 061 | (3 << 6);

	kbd_translate_table[1]['Z'] = 53 | (3 << 6);
	kbd_translate_table[1]['X'] = 54 | (3 << 6);
	kbd_translate_table[1]['C'] = 55 | (3 << 6);
	kbd_translate_table[1]['V'] = 56 | (3 << 6);
	kbd_translate_table[1]['B'] = 57 | (3 << 6);
	kbd_translate_table[1]['N'] = 58 | (3 << 6);
	kbd_translate_table[1]['M'] = 59 | (3 << 6);
	kbd_translate_table[1]['<'] = 60 | (3 << 6);
	kbd_translate_table[1]['>'] = 61 | (3 << 6);
	kbd_translate_table[1]['?'] = 62 | (3 << 6);

	// Map Delete to Rubout.
	kbd_translate_table[0][0x7f] = 046;
	kbd_translate_table[0][0x08] = 046;

	// Map Tab to Tab.
	kbd_translate_table[0][9] = 0211;

	// Map Escape to Escape.
	kbd_translate_table[0][0x1b] = 0204;
}

void
kbd_init(void)
{
	kbd_old_init();
}
