#include "driverEP1.h"

int processa(short int *M, int memSize)
{
    unsigned short int ri, pc, a, b, c, d, r, psw;
    a = 1;
    pc = 0;
    do
    {
        ri = M[pc];
        unsigned short int opcode = (ri & 0xF000) >> 12;
        unsigned short int arg = (ri & 0x0FFF);

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
            r = (pc + 1 >= memSize) ? 0 : pc + 1;
            pc = arg;
            break;
        case 0x0004:
            // JNZ X
            puts("JNZ");
            if(a != 0) {
                r = pc + 1;
                pc = arg;
            } else
                pc++;
            break;
        case 0x0005:
            //TODO: substituir newR por um dos registradores se eles não forem lidos por outras ops
            puts("RET");
            unsigned short int newR = pc + 1;
            pc = r;
            r = newR > memSize ? 0 : newR;
            break;
        case 0x0006:
            puts("ARIT");
            unsigned short int* regs[] = {&a, &b, &c, &d, 0, 0, &r, 0};
            unsigned short int arit = (arg & 0b111000000000) >> 9;
            unsigned short int resi = (arg & 0b000111000000) >> 6;
            unsigned short int op1i = (arg & 0b000000111000) >> 3;
            unsigned short int op2i = (arg & 0b000000000111);
            unsigned short int op1 = *regs[op1i];
            unsigned short int op2 = *regs[op2i];
            unsigned short int* res = regs[resi];


            printf("%hx %hx %hx %hx", arit, res, op1, op2);

            //TODO qual é o registrador x?
            //TODO PSW, underflow e overflow
            if (res != 0)
            switch(arit) {
                case 0:
                    *res = 0xFFFF;
                    break;
                case 1:
                    *res = 0;
                    break;
                case 2:
                    *res = ~op1;
                    break;
                case 3:
                    *res = op1 & op2;
                    break;
                case 4:
                    *res = op1 | op2;
                    break;
                case 5:
                    *res = op1 ^ op2;
                    break;
                case 6:
                    //OVERFLOW: como tratar?
                    *res = op1 + op2;
                    break;
                case 7:
                    //UNDERFLOW: como tratar?
                    *res = op1 - op2;
                    break;
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
    } while ((ri & 0xF000) != 0xF000);
}