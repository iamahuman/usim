#ifndef USIM_CC_H
#define USIM_CC_H

extern int cc_send(unsigned char *b, int len);
extern uint16_t reg_get(int base, int reg);
extern int reg_set(int base, int reg, int v);
extern uint16_t cc_get(int reg);
extern int cc_set(int reg, int v);
extern uint16_t mmc_get(int reg);
extern int mmc_set(int reg, int v);
extern uint32_t cc_read_obus_(void);
extern uint16_t cc_read_scratch(void);
extern uint32_t bitmask(int wid);
extern uint64_t ir_pair(int field, uint32_t val);
extern int cc_write_md_1s(void);
extern int cc_write_md_0s(void);
extern int cc_report_basic_regs(void);
extern int cc_report_pc(uint32_t *ppc);
extern int cc_report_pc_and_md(uint32_t *ppc);
extern int cc_report_pc_and_ir(uint32_t *ppc);
extern int cc_report_pc_md_ir(uint32_t *ppc);
extern int cc_report_status(void);
extern int cc_pipe(void);
extern int cc_pipe2(void);
extern int cc_setup_map(void);
extern int cc_report_ide_regs(void);
extern int _test_scratch(uint16_t v);
extern int cc_test_scratch(void);
extern int _test_ir(uint64_t isn);
extern int cc_test_ir(void);

#endif
