// See SYS; LCADR: LCADRD LISP and SYS;LCADR:LCADMC LISP for details.

#include <stdio.h>
#include <stdint.h>

#include "ucode.h"
#include "cc.h"

#include "lcadmc.h"
#include "lcadrd.h"

// Routines which manipulate the machine directly.

uint32_t cc_read_obus(void)
{
	return _cc_read_pair(SPY_OB_HIGH, SPY_OB_LOW);
}

uint32_t cc_read_a_bus(void)
{
	return _cc_read_pair(SPY_A_HIGH, SPY_A_LOW);
}

uint32_t cc_read_m_bus(void)
{
	return _cc_read_pair(SPY_M_HIGH, SPY_M_LOW);
}

uint32_t cc_read_ir(void)
{
	return _cc_read_triple(SPY_IR_HIGH, SPY_IR_MED, SPY_IR_LOW);
}

uint32_t cc_read_pc(void)
{
	return cc_get(SPY_PC);
}

uint32_t cc_read_status(void)
{
	uint16_t v1;
	uint16_t v2;
	uint16_t v3;

	v1 = cc_get(SPY_FLAG_1);
	v2 = cc_get(SPY_FLAG_2);
	v3 = cc_get(SPY_IR_LOW);

	if (v3 & 0100)
		v2 ^= 4;	// Hardware reads JC-TRUE incorrectly.

	return (v1 << 16) | v2;
}

void cc_write_diag_ir(ucw_t ir)
{
#if 0
	if (debug || verbose)
		disassemble_ucode_loc(ir);
#endif

	cc_set(SPY_IR_HIGH, (uint16_t) ((ir >> 32) & 0xffff));
	cc_set(SPY_IR_MED, (uint16_t) ((ir >> 16) & 0xffff));
	cc_set(SPY_IR_LOW, (uint16_t) ((ir >> 0) & 0xffff));

	cc_set(SPY_IR_HIGH, (uint16_t) ((ir >> 32) & 0xffff));
	cc_set(SPY_IR_MED, (uint16_t) ((ir >> 16) & 0xffff));
	cc_set(SPY_IR_LOW, (uint16_t) ((ir >> 0) & 0xffff));
}

void cc_write_ir(ucw_t ir)
{
#if 0
	if (debug || verbose)
		printf("ir %" PRIu64 " (0x%016llx)\n", ir, ir);
#endif
	cc_write_diag_ir(ir);
	cc_noop_debug_clock();
}

void cc_write_md(uint32_t num)
{
	cc_set(SPY_MD_HIGH, (uint16_t) (num >> 16) & 0xffff);
	cc_set(SPY_MD_LOW, (uint16_t) (num >> 0) & 0xffff);

	while (1) {
		uint32_t v;

		v = cc_read_md();
		if (v == num)
			break;

		printf("md readback failed, retry got %x want %x\n", v, num);
		cc_set(SPY_MD_HIGH, (uint16_t) (num >> 16) & 0xffff);
		cc_set(SPY_MD_LOW, (uint16_t) (num >> 0) & 0xffff);
	}
}

uint32_t cc_read_md(void)
{
	return _cc_read_pair(SPY_MD_HIGH, SPY_MD_LOW);
}

void cc_write_vma(uint32_t val)
{
	cc_set(SPY_VMA_HIGH, (uint16_t) (val >> 16) & 0xffff);
	cc_set(SPY_VMA_LOW, (uint16_t) (val >> 0) & 0xffff);

	while (cc_read_vma() != val) {
		printf("vma readback failed, retry\n");
		cc_set(SPY_VMA_HIGH, (uint16_t) (val >> 16) & 0xffff);
		cc_set(SPY_VMA_LOW, (uint16_t) (val >> 0) & 0xffff);
	}
}

uint32_t cc_read_vma(void)
{
	return _cc_read_pair(SPY_VMA_HIGH, SPY_VMA_LOW);
}

void cc_debug_clock (void)
{
	cc_set(SPY_CLK, 012);
	cc_set(SPY_CLK, 0);
}

void cc_noop_debug_clock (void)
{
	cc_set(SPY_CLK, 016);
	cc_set(SPY_CLK, 0);
}

void cc_clock (void)
{
	cc_set(SPY_CLK, 2);
	cc_set(SPY_CLK, 0);
}

void cc_noop_clock (void)
{
	cc_set(SPY_CLK, 6);
	cc_set(SPY_CLK, 0);
}

void cc_single_step (void)
{
	cc_set(SPY_CLK, 6);
	cc_set(SPY_CLK, 0);
}

// Routine to execute a symbolic instruction.

// Call these via the CC_EXECUTE function.

void cc_execute_r (ucw_t ir)
{
#if 0
	if (debug || verbose)
		printf("ir %" PRIu64 " (0x%016" PRIx64 ")\n", ir, ir);
#endif

again:
	cc_write_diag_ir(ir);
	cc_noop_debug_clock();
	if (cc_read_ir() != ir) {
		printf("ir reread failed; retry\n");
		goto again;
	}

	cc_debug_clock();
}

void cc_execute_w (ucw_t ir)
{
#if 0
	if (debug || verbose)
		printf("ir %" PRIu64 " (0x%016" PRIx64 ")\n", ir, ir);
#endif

again:
	cc_write_diag_ir(ir);
	cc_noop_debug_clock();
	if (cc_read_ir() != ir) {
		printf("ir reread failed; retry\n");
		goto again;
	}

	cc_clock();
	cc_noop_clock();
}

// Read and write RAMs.

uint32_t cc_read_m_mem (uint32_t adr)
{
	cc_execute(0,
		   ir_pair(CONS_IR_M_SRC, adr) |
		   ir_pair(CONS_IR_ALUF, CONS_ALU_SETM) |
		   ir_pair(CONS_IR_OB, CONS_OB_ALU));

	return cc_read_obus();
}


void cc_write_m_mem (uint32_t loc, uint32_t val)
{
	///---!!!
}

uint32_t cc_read_a_mem (uint32_t adr)
{
	cc_execute(0,
		   ir_pair(CONS_IR_A_SRC, adr) |
		   ir_pair(CONS_IR_ALUF, CONS_ALU_SETA) |
		   ir_pair(CONS_IR_OB, CONS_OB_ALU));

	return cc_read_obus();
}

void cc_write_a_mem (uint32_t loc, uint32_t val)
{
	uint32_t v2;

	cc_write_md(val);
	v2 = cc_read_md();
	if (v2 != val) {
		printf("cc_write_a_mem; md readback error (got=%o wanted=%o)\n", v2, val);
	}
	cc_execute(WRITE,
		   ir_pair(CONS_IR_M_SRC, CONS_M_SRC_MD) |
		   ir_pair(CONS_IR_ALUF, CONS_ALU_SETM) |
		   ir_pair(CONS_IR_OB, CONS_OB_ALU) |
		   ir_pair(CONS_IR_A_MEM_DEST, CONS_A_MEM_DEST_INDICATOR + loc));
}

// Reset, start and stop.

void cc_stop_mach (void)
{
	cc_set(SPY_CLK, 0);
}

void cc_start_mach (void)
{
	cc_set(SPY_CLK, 0001);
}
