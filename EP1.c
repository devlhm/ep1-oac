#include "driverEP1.h"

char itoregname(int i) {
	char regnames[] = {'A', 'B', 'C', 'D', '0', '0', 'R', '0', 'X'};
	return regnames[i];
}

int processa(short int *M, int memSize)
{
	unsigned short int ri, pc, a, b, c, d, r, psw;
	pc = 0;
	do
	{
		ri = M[pc];
		unsigned short int opcode = (ri & 0xF000) >> 12;
		unsigned short int arg = (ri & 0x0FFF);

		printf("PC: %hx\n", pc);
		printf("REGS: a = %hx | b = %hx | c = %hx | d = %hx | r = %hx | psw = %hx\n", a, b, c, d, r, psw);
		printf("Palavra %hx -> OP: %hx | X: %hx\n", ri, opcode, arg);

		switch (opcode)
		{
		case 0x0000:
			// NOP
			puts("NOP");
			break;
		case 0x0001:
			// LDA(X)
			puts("LDA");
			a = M[arg];
			printf("Valor lido: %hx\n", a);
			break;
		case 0x0002:
			// STA(X)
			puts("STA");
			M[arg] = a;
			printf("Valor carregado: %hx\n", a);
			break;
		case 0x0003:
			// JMP X
			puts("JMP");
			r = pc + 1;
			pc = arg;
			break;
		case 0x0004:
			// JNZ X
			puts("JNZ");
			if (a != 0)
			{
				r = pc + 1;
				pc = arg;
			}
			else
				pc++;

			break;
		case 0x0005:
			// TODO: substituir newR por um dos registradores se eles não forem lidos por outras ops
			puts("RET");
			unsigned short int newR = pc + 1;
			pc = r;
			r = newR > memSize ? 0 : newR;
			break;
		case 0x0006:
			puts("ARIT");
			unsigned short int *regs[] = {&a, &b, &c, &d, 0, 0, &r, 0};
			unsigned short int arit = (arg & (0x7 << 9)) >> 9;
			unsigned short int resi = (arg & (0x7 << 6)) >> 6;
			unsigned short int op1i = (arg & (0x7 << 3)) >> 3;
			unsigned short int op2i = (arg & 0x007);
			unsigned short int op1 = regs[op1i] == 0 ? 0 : *regs[op1i];
			unsigned short int op2;

			if (!(op2i & 0b000000000100)) {
				op2i = 8;
				op2 = 0;
			} else {
				printf("op2i: %hx\n", op2i);
				op2 = *regs[op2i > 1];
			}

			unsigned short int *res = regs[resi];
			printf("AritOp: %hx | Res: %c | op1: %c | op2: %c\n", arit, itoregname(resi), itoregname(op1i), itoregname(op2i));
			

			// TODO por algum motivo, o programa de exemplo ta gerando um loop infinito de jumps. culpa minha ou do programa?
			// TODO qual é o regx?
			if (res != (unsigned short int) 0) {
				switch (arit)
				{
				case 0:
					printf("Resultado FFFF ");
					*res = 0xFFFF;
					break;
				case 1:
					printf("Resultado 0 ");
					*res = 0;
					break;
				case 2:
					printf("Nega %hx ", op1);
					*res = ~op1;
					break;
				case 3:
					printf("and(%hx, %hx) ", op1, op2);
					*res = op1 & op2;
					break;
				case 4:
					printf("or(%hx, %hx) ", op1, op2);
					*res = op1 | op2;
					break;
				case 5:
					printf("xor(%hx, %hx) ", op1, op2);
					*res = op1 ^ op2;
					break;
				case 6:
					printf("add(%hx, %hx) ", op1, op2);
					if (op1 > 0xffff - op2)
						psw = psw | ((unsigned short int)1 << 15);
					if (op1 < op2)
						psw = psw | ((unsigned short int)1 << 13);
					else if (op1 == op2)
						psw = psw | ((unsigned short int)1 << 12);
					else
						psw = psw | ((unsigned short int)1 << 11);

					*res = op1 + op2;

					break;
				case 7:
					printf("sub(%hx, %hx) ", op1, op2);
					if (op1 > 0xffff + op2)
						psw = psw | ((unsigned short int)1 << 14);
					if (op1 < op2)
						psw = psw | ((unsigned short int)1 << 13);
					else if (op1 == op2)
						psw = psw | ((unsigned short int)1 << 12);
					else
						psw = psw | ((unsigned short int)1 << 11);

					*res = op1 - op2;
					break;
				}
				printf("= %hx\n", *res);
			}
			break;
		case 0x000f:
			// HALT
			puts("HALT");
			break;

		default:
			break;
		}
		if (opcode != 0x0003 && opcode != 0x0004)
			pc++;
		if (pc >= memSize)
			pc = 0;

		char a;
		scanf("%c", &a);
		puts("");
	} while ((ri & 0xF000) != 0xF000);
}