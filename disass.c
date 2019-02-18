// disass.c --- ---!!!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>

#include "usim.h"
#include "ucode.h"
#include "disass.h"
#include "syms.h"
#include "misc.h"

#include "macroops.h"
#include "microops.h"

void
disassemble_m_src(ucw_t u, int m_src)
{
	if (m_src & 040) {
		switch (m_src & 037) {
		case 0:
			printf("dispatch-constant ");
			break;
		case 1:
			printf("SPC-ptr, spc-data ");
			break;
		case 2:
			printf("PDL-ptr %o ", (int) u & 01777);
			break;
		case 3:
			printf("PDL-index %o ", (int) u & 01777);
			break;
		case 5:
			printf("PDL-buffer ");
			break;
		case 6:
			printf("OPC register %o ", (int) u & 017777);
			break;
		case 7:
			printf("Q ");
			break;
		case 010:
			printf("VMA ");
			break;
		case 011:
			printf("MAP[MD] ");
			break;
		case 012:
			printf("MD ");
			break;
		case 013:
			printf("LC ");
			break;
		case 014:
			printf("SPC pointer and data, pop ");
			break;
		case 024:
			printf("PDL[Pointer], pop ");
			break;
		case 025:
			printf("PDL[Pointer] ");
			break;
		}
	} else {
		printf("m[%o] ", m_src);
	}
}

void
disassemble_dest(int dest)
{
	if (dest & 04000) {
		printf("->a_mem[%o] ", dest & 01777);
	} else {
		switch (dest >> 5) {
		case 0:
			printf("-><none>");
			break;
		case 1:
			printf("->LC ");
			break;
		case 2:
			printf("->IC ");
			break;
		case 010:
			printf("->PDL[ptr] ");
			break;
		case 011:
			printf("->PDL[ptr],push ");
			break;
		case 012:
			printf("->PDL[index] ");
			break;
		case 013:
			printf("->PDL index ");
			break;
		case 014:
			printf("->PDL ptr ");
			break;
		case 015:
			printf("->SPC data,push ");
			break;
		case 016:
			printf("->OA-reg-lo ");
			break;
		case 017:
			printf("->OA-reg-hi ");
			break;
		case 020:
			printf("->VMA ");
			break;
		case 021:
			printf("->VMA,start-read ");
			break;
		case 022:
			printf("->VMA,start-write ");
			break;
		case 023:
			printf("->VMA,write-map ");
			break;
		case 030:
			printf("->MD ");
			break;
		case 031:
			printf("->MD,start-read ");
			break;
		case 032:
			printf("->MD,start-write ");
			break;
		case 033:
			printf("->MD,write-map ");
			break;
		}
		printf(",m[%o] ", dest & 037);
	}
}

void
disassemble_ucode_loc(ucw_t u)
{
	int a_src;
	int m_src;
	int new_pc;
	int dest;
	int alu_op;
	int r_bit;
	int p_bit;
	int n_bit;
	int ir8;
	int ir7;
	int widthm1;
	int pos;
	int mr_sr_bits;
	int disp_cont;
	int disp_addr;
	int map;
	int len;
	int rot;
	int out_bus;

	if ((u >> 42) & 1)
		printf("popj; ");

	switch ((u >> 43) & 03) {
	case 0:		// ALU.
		printf("(alu) ");

		if ((u & NOP_MASK) == 0) {
			printf("no-op");
			goto done;
		}

		a_src = (u >> 32) & 01777;
		m_src = (u >> 26) & 077;
		dest = (u >> 14) & 07777;
		out_bus = (u >> 12) & 3;
		ir8 = (u >> 8) & 1;
		ir7 = (u >> 7) & 1;

		alu_op = (u >> 3) & 017;
		if (ir8 == 0) {
			if (ir7 == 0) {
				printf("%s ", alu_bool_op[alu_op]);
			} else {
				printf("%s ", alu_arith_op[alu_op]);
			}
		} else {
			switch (alu_op) {
			case 0:
				printf("mult-step ");
				break;
			case 1:
				printf("div-step ");
				break;
			case 5:
				printf("rem-corr ");
				break;
			case 011:
				printf("init-div-step ");
				break;
			}
		}

		printf("a=%o m=%o ", a_src, m_src);
		disassemble_m_src(u, m_src);

		if ((u >> 2) & 1)
			printf("C=1 ");
		else
			printf("C=0 ");

		switch (out_bus) {
		case 1:
			printf("alu-> ");
			break;
		case 2:
			printf("alu>>+s ");
			break;
		case 3:
			printf("alu<<+q31 ");
			break;
		}

		switch (u & 3) {
		case 1:
			printf("<<Q ");
			break;
		case 2:
			printf(">>Q ");
			break;
		case 3:
			printf("Q-R ");
			break;
		}
		disassemble_dest(dest);
		break;
	case 1:		// JUMP.
		printf("(jump) ");
		a_src = (u >> 32) & 01777;
		m_src = (u >> 26) & 077;
		new_pc = (u >> 12) & 037777;

		printf("a=%o m=", a_src);
		disassemble_m_src(u, m_src);

		r_bit = (u >> 9) & 1;
		p_bit = (u >> 8) & 1;
		n_bit = (u >> 7) & 1;

		printf("pc %o, %s%s", new_pc, r_bit ? "R " : "", p_bit ? "P " : "");
		if (n_bit)	// INHIBIT-XCT-NEXT
			printf("!next ");

		if (u & (1 << 6))	// INVERT-JUMP-SENSE
			printf("!jump ");

		if (u & (1 << 5)) {
			switch (u & 017) {
			case 0:
			case 1:
				printf("M-src < A-src ");
				break;
			case 2:
				printf("M-src <= A-src ");
				break;
			case 3:
				printf("M-src = A-src ");
				break;
			case 4:
				printf("pf ");
				break;
			case 5:
				printf("pf/int ");
				break;
			case 6:
				printf("pf/int/seq ");
				break;
			case 7:
				printf("jump-always ");
				break;
			}
		} else {
			printf("m-rot<< %o", (int) u & 037);
		}
		break;
	case 2:		// DISPATCH.
		printf("(dispatch) ");
		disp_cont = (u >> 32) & 01777;
		m_src = (u >> 26) & 077;

		if ((u >> 25) & 1)
			printf("!N+1 ");
		if ((u >> 24) & 1)
			printf("ISH ");
		disp_addr = (u >> 12) & 03777;
		map = (u >> 8) & 3;
		len = (u >> 5) & 07;
		rot = u & 037;

		printf("m=%o ", m_src);
		disassemble_m_src(u, m_src);

		printf("disp-const %o, disp-addr %o, map %o, len %o, rot %o ", disp_cont, disp_addr, map, len, rot);
		break;
	case 3:		// BYTE.
		printf("(byte) ");
		a_src = (u >> 32) & 01777;
		m_src = (u >> 26) & 077;
		dest = (u >> 14) & 07777;
		mr_sr_bits = (u >> 12) & 3;

		widthm1 = (u >> 5) & 037;
		pos = u & 037;

		printf("a=%o m=", a_src);
		disassemble_m_src(u, m_src);

		switch (mr_sr_bits) {
		case 0:
			break;
		case 1:	// LDB.
			printf("ldb pos=%o, width=%o ", pos, widthm1 + 1);
			break;
		case 2:
			printf("sel dep (a<-m&mask) pos=%o, width=%o ", pos, widthm1 + 1);
			break;
		case 3:	// DPB.
			printf("dpb pos=%o, width=%o ", pos, widthm1 + 1);
			break;
		}

		disassemble_dest(dest);
		break;
	}

done:
	printf("\n");
}

void
disassemble_instruction(unsigned int fefptr, unsigned int loc, int even, unsigned int inst, unsigned int width)
{
	int op;
	int dest;
	int reg;
	int delta;
	int adr;
	int to;
	unsigned int nlc;

	op = (inst >> 011) & 017;
	dest = (inst >> 015) & 07;
	reg = (inst >> 6) & 07;
	delta = (inst >> 0) & 077;
	printf("%011o%c %06o %s ", loc, even ? 'e' : 'o', inst, op_names[op]);

	switch (op) {
	case 0:		// CALL
		printf("reg %s, ", reg_names[reg]);
		printf("dest %s, ", dest_names[dest]);
		printf("delta %o ", delta);
		{
			unsigned int v;
			unsigned int tag;

			v = read_virt(fefptr + delta);
			tag = (v >> width) & 037;
			switch (tag) {
			case 3:
				v = read_virt(v);
				showstr(v);
				break;
			case 4:
				showstr(v);
				break;
			case 027:
				break;
			default:
				v = read_virt(v);
				show_fef_func_name(v, width);
				break;
			}
		}
		break;
	case 2:		// MOVE.
	case 3:		// CAR
	case 4:		// CDR.
	case 5:		// CADR.
		printf("reg %s, ", reg_names[reg]);
		printf("dest %s, ", dest_names[dest]);
		printf("delta %o ", delta);
		break;
	case 011:		// ND1.
		printf("%s ", nd1_names[dest]);
		break;
	case 012:		// ND2.
		printf("%s ", nd2_names[dest]);
		break;
	case 013:		// ND3.
		printf("%s ", nd3_names[dest]);
		break;
	case 014:		// BRANCH.
		printf("type %s, ", branch_names[dest]);

		to = (inst & 03777) << 1;
		to |= (inst & 0x8000) ? 1 : 0;

		if (inst & 0400) {
			to = inst & 01777;
			to |= 03000;
			to |= ~01777;
		}

		nlc = (loc * 2 + (even ? 0 : 1)) + to;

		if (to > 0) {
			printf("+%o; %o%c ", to, nlc / 2, (nlc & 1) ? 'o' : 'e');
		} else {
			printf("-%o; %o%c ", -to, nlc / 2, (nlc & 1) ? 'o' : 'e');
		}
		break;
	case 015:		// MISC.
		adr = inst & 0777;
		printf("%o ", adr);
		printf("dest %s, ", dest_names[dest]);
		break;
	}
	printf("\n");
}

void
showstr(int a)
{
	int t;
	int j;
	char s[256];

	t = read_virt(a) & 0xff;
	j = 0;
	for (int i = 0; i < t; i += 4) {
		unsigned int n;

		n = read_virt(a + 1 + (i / 4));
		s[j++] = n >> 0;
		s[j++] = n >> 8;
		s[j++] = n >> 16;
		s[j++] = n >> 24;
	}
	s[t] = 0;
	printf("'%s' ", s);
}

void
show_fef_func_name(unsigned int fefptr, unsigned int width)
{
	unsigned int n;
	unsigned int v;
	int tag;

	n = read_virt(fefptr + 2);
	printf(" ");
	v = read_virt(n);
	tag = (v >> width) & 037;
	if (tag == 3) {
		v = read_virt(v);
		tag = (v >> width) & 037;
	}
	if (tag == 4) {
		printf(" ");
		showstr(v);
	}
}
