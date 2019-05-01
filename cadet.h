#ifndef USIM_CADET_H
#define USIM_CADET_H

#include <stdint.h>

#include "lmk.h"

// See SYS:LMWIN;COLD LISP for details.
//
// The second dimension is 200 long and indexed by keycode.
// The first dimension is the shifts:
//  0 unshifted
//  1 shift
//  2 top
//  3 greek
//  4 shift greek
// Elements in the table are 16-bit unsigned numbers.
// Bit 15 on and bit 14 on means undefined code, ignore and beep.
// Bit 15 on and bit 14 off means low bits are shift for bit in KBD-SHIFTS
//    (40 octal for right-hand key of a pair)
// Bit 15 off is ordinary code.
uint16_t kbd_new_table[0200][5] = {
	{},			// 0 not used
	{LMK_roman_ii},		// 1 Roman II
	{LMK_roman_iv},		// 2 Roman IV
	{100011},		// 3 Mode lock
	{100006},		// 5 Left super
	{LMK_4, LMK_dollar, LMK_dollar},    // 11 Four
	{LMK_r, LMK_R, LMK_down_horseshoe}, // 12 R
	{LMK_f, LMK_F},			    // 13 F
	{LMK_v, LMK_V},			    // 14 V
	{100008},		// 15 Alt Lock
	{LMK_hand_right},	// 17 Hand Right
	{100004},		// 20 Left control
	{LMK_colon, LMK_plus_minus, LMK_plus_minus}, // 21 plus-minus
	{LMK_tab},				     // 22 tab
	{LMK_rubout},				     // 23 rubout
	{100000},				   // 24 Left Shift
	{100040},		// 25 Right Shift
	{100044},		// 26 Right control
	{LMK_hold_output},		     // 30 hold output
	{LMK_8, LMK_asterisk, LMK_asterisk}, // 31 Eight
	{LMK_i, LMK_I, LMK_infinity},	     // 32 I
	{LMK_k, LMK_K, LMK_right_arrow},     // 33 K
	{LMK_comma, LMK_less, LMK_less},     // 34 comma
	{100041},		// 35 Right Greek
	{LMK_line},			   // 36 Line
	{LMK_backslash, LMK_bar, LMK_bar}, // 37 Backslash
	{LMK_esc},			   // 40 terminal
	{LMK_network},			   // 42 network
	{100001},				   // 44 Left Greek
	{100005},		// 45 Left Meta
	{LMK_status},				   // 46 status
	{LMK_resume},				   // 47 resume
	{LMK_form},				   // 50 clear screen
	{LMK_6, LMK_asciicircum, LMK_asciicircum}, // 51 Six
	{LMK_y, LMK_Y, LMK_right_horseshoe},	   // 52 Y
	{LMK_h, LMK_H, LMK_down_arrow},		   // 53 H
	{LMK_n, LMK_N, LMK_less_or_equal},	   // 54 N
	{LMK_2, LMK_at, LMK_at},		   // 61 Two
	{LMK_w, LMK_W, LMK_or_sign},		   // 62 W
	{LMK_s, LMK_S},				   // 63 S
	{LMK_x, LMK_X},				   // 64 X
	{100046},		// 65 Right Super
	{LMK_abort},				       // 67 Abort
	{LMK_9, LMK_parenleft, LMK_parenleft},	       // 71 Nine
	{LMK_o, LMK_O, LMK_existential_quantifier},    // 72 O
	{LMK_l, LMK_L, LMK_partial_delta, LMK_lambda}, // 73 L/lambda
	{LMK_period, LMK_greater, LMK_greater},	       // 74 period
	{LMK_grave, LMK_asciitilde, LMK_asciitilde, LMK_not_sign}, // 77 back quote
	{LMK_back_next},	// 100 macro
	{LMK_roman_i},		// 101 Roman I
	{LMK_roman_iii},	// 102 Roman III
	{100002},				   // 104 Left Top
	{LMK_hand_up},				   // 106 Up Thumb
	{LMK_call},				   // 107 Call
	{LMK_clear},				   // 110 Clear Input
	{LMK_5, LMK_percent, LMK_percent},	   // 111 Five
	{LMK_t, LMK_T, LMK_left_horseshoe},	   // 112 T
	{LMK_g, LMK_G, LMK_up_arrow, LMK_gamma},   // 113 G/gamma
	{LMK_b, LMK_B, LMK_equivalence, LMK_beta}, // 114 B
	{},			// 115 Repeat
	{LMK_help},		// 116 Help
	{LMK_hand_left, LMK_hand_left, LMK_hand_left, LMK_circle_x, LMK_circle_x}, // 117 Hand Left
	{LMK_quote},			   // 120 Quote
	{LMK_1, LMK_exclam, LMK_exclam},   // 121 One
	{LMK_q, LMK_Q, LMK_and_sign},	   // 122 Q
	{LMK_a, LMK_A, 140000, LMK_alpha}, // 123 A
	{LMK_z, LMK_Z},			   // 124 Z
	{100003},				   // 125 Caps Lock
	{LMK_equal, LMK_plus, LMK_plus},	     // 126 Equals
	{LMK_minus, LMK_underscore, LMK_underscore}, // 131 Minus
	{LMK_parenleft, LMK_bracketleft, LMK_bracketleft}, // 132 Open parenthesis
	{LMK_apostrophe, LMK_quotedbl, LMK_quotedbl, LMK_center_dot}, // 133 Apostrophe/center-dot
	{LMK_sp},		// 134 Space
	{LMK_cr},		// 136 Return
	{LMK_parenright, LMK_bracketright, LMK_bracketright}, // 137 Close parenthesis
	{LMK_system},		// 141 system
	{LMK_altmode},		// 143 Alt Mode
	{100007},		// 145 Left Hyper
	{LMK_braceright, 140000, 140000},	  // 146 }
	{LMK_7, LMK_ampersand, LMK_ampersand},	  // 151 Seven
	{LMK_u, LMK_U, LMK_universal_quantifier}, // 152 U
	{LMK_j, LMK_J, LMK_left_arrow},		  // 153 J
	{LMK_m, LMK_M, LMK_greater_or_equal},	  // 154 M
	{100042},		// 155 Right Top
	{LMK_end},				 // 156 End
	{LMK_delete},				 // 157 Delete
	{LMK_bs},				 // 160 Overstrike
	{LMK_3, LMK_numbersign, LMK_numbersign}, // 161 Three
	{LMK_e, LMK_E, LMK_up_horseshoe, LMK_epsilon}, // 162 E
	{LMK_d, LMK_D, 140000, LMK_delta},	       // 163 D/delta
	{LMK_c, LMK_C, LMK_not_equal},		       // 164 C
	{100045},		// 165 Right Meta
	{LMK_braceleft, 140000, 140000},	   // 166 {
	{LMK_break},				   // 167 Break
	{LMK_stop_output},			   // 170 Stop Output
	{LMK_0, LMK_parenright, LMK_parenright},   // 171 Zero
	{LMK_p, LMK_P, LMK_partial_delta, LMK_pi}, // 172 P
	{LMK_semicolon, LMK_colon, LMK_colon},	   //  173 Semicolon
	{LMK_slash, LMK_question, LMK_question, LMK_integral}, // 174 Question/Integral
	{100047},		// 175 Right Hyper
	{LMK_hand_down, LMK_hand_down, LMK_hand_down, /* #/^?^M, #/^?^M */}, // 176 Down Thumb
};

#endif
