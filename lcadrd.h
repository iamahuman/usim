#include <stdint.h>

#include "ucode.h"

enum {				// Reading
	SPY_IR_LOW      = 000,
	SPY_IR_MED      = 001,
	SPY_IR_HIGH     = 002,
	SPY_SCRATCH     = 003,	///---!!!
	SPY_OPC         = 004,
	SPY_PC          = 005,
	SPY_OB_LOW      = 006,
	SPY_OB_HIGH     = 007,
	SPY_FLAG_1      = 010,
	SPY_FLAG_2      = 011,
	SPY_M_LOW       = 012,
	SPY_M_HIGH      = 013,
	SPY_A_LOW       = 014,
	SPY_A_HIGH      = 015,
	SPY_STAT_LOW    = 016,
	SPY_STAT_HIGH   = 017,

	SPY_MD_LOW      = 020,	///---!!!
	SPY_MD_HIGH     = 021,	///---!!!
	SPY_VMA_LOW     = 022,	///---!!!
	SPY_VMA_HIGH    = 023,	///---!!!
	SPY_DISK        = 026,	///---!!!
	SPY_BD          = 027,	///---!!!
};

enum {				// WRITING
	//SPY_IR_LOW    = 0,
	//SPY_IR_MED    = 1,
	//SPY_IR_HIGH   = 2,
	SPY_CLK         = 3,
	SPY_OPC_CONTROL = 4,
	SPY_MODE        = 5,
};

uint32_t cc_read_obus(void);
uint32_t cc_read_a_bus(void);
uint32_t cc_read_m_bus(void);
uint32_t cc_read_ir(void);
uint32_t cc_read_pc(void);
uint32_t cc_read_status(void);
void cc_write_diag_ir(ucw_t ir);
void cc_write_ir(ucw_t ir);
void cc_write_md(uint32_t num);
uint32_t cc_read_md(void);
void cc_write_vma(uint32_t val);
uint32_t cc_read_vma(void);

void cc_debug_clock(void);
void cc_noop_debug_clock(void);
void cc_clock(void);
void cc_noop_clock(void);
void cc_single_step(void);

void cc_execute_r(ucw_t ir);
void cc_execute_w(ucw_t ir);

uint32_t cc_read_m_mem(uint32_t adr);
void cc_write_m_mem(uint32_t loc, uint32_t val);
uint32_t cc_read_a_mem(uint32_t adr);
void cc_write_a_mem(uint32_t loc, uint32_t val);

void cc_stop_mach(void);
void cc_start_mach(void);
