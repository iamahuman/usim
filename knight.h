#ifndef USIM_KNIGHT_H
#define USIM_KNIGHT_H

#include "lmk.h"

// See SYS:LMWIN;COLD LISP for details.
//
// Keyboard translate table is a 3 X 64 array.
// 3 entries for each of 100 keys.  First is vanilla, second shift, third top.
unsigned char kbd_old_table[64][3] = {
	{LMK_break, LMK_break, LMK_network},
	{LMK_esc, LMK_esc, LMK_system},
	{LMK_1, LMK_exclam, LMK_exclam},
	{LMK_2, LMK_quotedbl, LMK_quotedbl},
	{LMK_3, LMK_numbersign, LMK_numbersign},
	{LMK_4, LMK_dollar, LMK_dollar},
	{LMK_5, LMK_percent, LMK_percent},
	{LMK_6, LMK_ampersand, LMK_ampersand},
	{LMK_7, LMK_apostrophe, LMK_apostrophe},
	{LMK_8, LMK_parenleft, LMK_parenleft},
	{LMK_9, LMK_parenright, LMK_parenright},
	{LMK_0, LMK_underscore, LMK_underscore},
	{LMK_minus, LMK_equal, LMK_equal},
	{LMK_at, LMK_grave, LMK_grave},
	{LMK_asciicircum, LMK_asciitilde, LMK_asciitilde},
	{LMK_bs, LMK_bs, LMK_bs},
	{LMK_call, LMK_call, LMK_abort},

	{LMK_clear, LMK_clear, LMK_clear},
	{LMK_tab, LMK_tab, LMK_tab},
	{LMK_altmode, LMK_altmode, LMK_altmode},
	{LMK_q, LMK_Q, LMK_and_sign},
	{LMK_w, LMK_W, LMK_or_sign},
	{LMK_e, LMK_E, LMK_up_horseshoe},
	{LMK_r, LMK_R, LMK_down_horseshoe},
	{LMK_t, LMK_T, LMK_left_horseshoe},
	{LMK_y, LMK_Y, LMK_right_horseshoe},
	{LMK_u, LMK_U, LMK_not_sign},
	{LMK_i, LMK_I, LMK_circle_x},
	{LMK_o, LMK_O, LMK_down_arrow},
	{LMK_p, LMK_P, LMK_up_arrow},
	{LMK_bracketleft, LMK_braceleft, LMK_braceleft},
	{LMK_bracketright, LMK_braceright, LMK_braceright},
	{LMK_backslash, LMK_bar, LMK_bar},
	{LMK_slash, LMK_infinity, LMK_infinity},
	{LMK_plus_minus, LMK_delta, LMK_delta},
	{LMK_circle_plus, LMK_circle_plus, LMK_circle_plus},

	{LMK_form, LMK_form, LMK_form},
	{LMK_vt, LMK_vt, LMK_vt},
	{LMK_rubout, LMK_rubout, LMK_rubout},
	{LMK_a, LMK_A, LMK_less_or_equal},
	{LMK_s, LMK_S, LMK_greater_or_equal},
	{LMK_d, LMK_D, LMK_equivalence},
	{LMK_f, LMK_F, LMK_partial_delta},
	{LMK_g, LMK_G, LMK_not_equal},
	{LMK_h, LMK_H, LMK_help},
	{LMK_j, LMK_J, LMK_left_arrow},
	{LMK_k, LMK_K, LMK_right_arrow},
	{LMK_l, LMK_L, LMK_double_arrow},
	{LMK_semicolon, LMK_plus, LMK_plus},
	{LMK_colon, LMK_asterisk, LMK_asterisk},
	{LMK_cr, LMK_cr, LMK_end},
	{LMK_line, LMK_line, LMK_line},
	{LMK_back_next, LMK_back_next, LMK_back_next},

	// #\SHIFT-LOCK
	// #\LEFT-TOP
	// #\LEFT-SHIFT
	{LMK_z, LMK_Z, LMK_alpha},
	{LMK_x, LMK_X, LMK_beta},
	{LMK_c, LMK_C, LMK_epsilon},
	{LMK_v, LMK_V, LMK_lambda},
	{LMK_b, LMK_B, LMK_pi},
	{LMK_n, LMK_N, LMK_universal_quantifier},
	{LMK_m, LMK_M, LMK_existential_quantifier},
	{LMK_comma, LMK_less, LMK_less},
	{LMK_period, LMK_greater, LMK_greater},
	{LMK_slash, LMK_question, LMK_question},
	// #\RIGHT-SHIFT
	// #\RIGHT-TOP

	// #\LEFT-META
	// #\LEFT-CTRL
	{LMK_sp, LMK_sp, LMK_sp},
	// #\RIGHT-CTRL
	// #\RIGHT-META
};

#endif
