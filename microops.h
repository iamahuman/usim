#ifndef USIM_MICROOPS_H
#define USIM_MICROOPS_H

char *alu_bool_op[] = {
	"SETZ",
	"AND",
	"ANDCA",
	"SETM",
	"ANDCM",
	"SETA",
	"XOR",
	"IOR",
	"ANDCB",
	"EQV",
	"SETCA",
	"ORCA",
	"SETCM",
	"ORCM",
	"ORCB",
	"SETO"
};

char *alu_arith_op[] = {
	"-1",
	"(M&A)-1",
	"(M&~A)-1",
	"M-1",
	"M|~A",
	"(M|~A)+(M&A)",
	"M-A-1 [M-A-1]",
	"(M|~A)+M",
	"M|A",
	"M+A [ADD]",
	"(M|A)+(M&~A)",
	"(M|A)+M",
	"M",
	"M+(M&A)",
	"M+(M|~A)",
	"M+M"
};

#endif
