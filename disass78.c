// disass78.c -- macrocode disassembler for System 78
//
// See SYS: SYS2; DISASS LISP for details.

#define DISASSBUFSZ 128
static char dissbuf[DISASSBUFSZ];
static void *dissbufp;

char *(*disassemble_object_output_fun)(uint32_t, uint32_t) = NULL;

#define PRIN1(args...) do { dissbufp += sprintf(dissbufp, args); } while (0)
#define PRIN1SP(args...) do { PRIN1(args); PRIN1(" "); } while (0)

static char *constant_page[] = { "NIL", "T", "0", "1", "2" };
static int constant_page_size = sizeof(constant_page) / sizeof(constant_page[0]);

static void
constants_area(int addr)
{
	assert(addr < constant_page_size);
	PRIN1("%s", constant_page[addr]);
}

static void
disassemble_address(uint32_t fef, uint32_t reg, uint32_t disp)
{
	PRIN1(" ");

	if (reg < 4) {
		PRIN1("FEF|%d", disp);
		if (disassemble_object_output_fun != NULL) {
			char *s;

			s = disassemble_object_output_fun(fef, disp);
			if (s != NULL)
				PRIN1("\t\t; %s", s);
		}
	} else if (reg == 4) {
		PRIN1("'");
		constants_area(077 & disp);
	} else if (disp == 0777) {
		PRIN1("PDL-POP");
	} else if (reg == 5) {
		PRIN1("LOCAL|-%d", 077 & disp);
	} else if (reg == 6) {
		PRIN1("ARG|-%d", 077 & disp);
	} else {
		PRIN1("PDL|-%d", 077 & disp);
	}
}

char *
disassemble_instruction(uint32_t fef, uint32_t pc, uint32_t wd, uint32_t second_word)
{
	int op;
	int dest;
	int disp;
	int reg;

	defmics_init();

	dissbuf[0] = '\0';
	dissbufp = dissbuf;

	PRIN1("%011o %06o ", pc, wd);

	op = ldb(01104, wd);
	dest = ldb(01503, wd);
	disp = ldb(00011, wd);
	reg = ldb(00603, wd);

	if (wd == 0) {
		PRIN1("0");
	} else if (op < 011) {	// DEST/ADDR
		PRIN1("%s %s", call_names[op], dest_names[dest]);
		disassemble_address(fef, reg, disp);
	} else if (op == 011) {	// ND1
		PRIN1("%s", nd1_names[dest]);
		disassemble_address(fef, reg, disp);
	} else if (op == 012) { // ND2
		PRIN1("%s", nd2_names[dest]);
		disassemble_address(fef, reg, disp);
	} else if (op == 013) { // ND3
		PRIN1("%s", nd3_names[dest]);
		disassemble_address(fef, reg, disp);
	} else if (op == 014) { // BRANCH
		PRIN1SP("%s", branch_names[dest]);

		if (disp > 0400)
			disp |= ~0400; // SIGN-EXTEND

		if (disp != -1) // ONE WORD
			PRIN1("%o", pc + disp + 1);
		else {     // LONG BRANCH
			pc++;
			disp = second_word;
			if (disp > 0100000)
				disp |= ~0100000;
			PRIN1("*%o", pc + disp + 1); // INDICATE LONG BRANCH FOR USER.
		}
	} else if (op == 015) { // MISC
		PRIN1SP("(MISC)");
		if (disp < 0100) {
			PRIN1("LIST %d long ", disp);
		} else if (disp < 0200) {
			PRIN1("LIST-IN-AREA %d long ", disp - 0100);
		} else if (disp < 0220) {
			PRIN1("UNBIND %d binding ", disp - 0177); // code 200 does 1 unbind.
		} else if (disp < 0240) {
			PRIN1("POP-PDL %d time~ ", disp - 0220); // code 220 does 0 pops.
		} else {
			int d = disp; // - 0200;

			if (d < 1024 && defmics_vector[d])
				PRIN1SP("%s", defmics[defmics_vector[d]].name);
			else
				PRIN1SP("#%o / %d", disp, disp);
		}
		PRIN1("%s", dest_names[dest]);
	} else {		// UNDEF
		PRIN1("UNDEF-%d", op);
	}

	return dissbuf;
}
