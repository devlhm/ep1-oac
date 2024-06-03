#include "driverEP1.h"

int processa(short int *M, int memSize)
{
    unsigned short int ri, pc, a, b, c, d, r, psw;
    pc = 0;
    do
    {
        ri = M[pc];
        unsigned short int opcode = (ri & 0xF);
        unsigned short int arg = (ri & 0x0FFF);

        printf("%hx: %hx %hx\n", ri, opcode, arg);

        switch (opcode)
        {
        case 0b0000:
            // NOP
            puts("NOP");
            break;
        case 0b0001:
            // LDA(X)
            puts("LDA");
            break;
        case 0b0010:
            // STA(X)
            puts("STA");
            break;
        case 0b0011:
            // JMP X
            puts("JMP");
            break;
        case 0b0100:
            // JNZ X
            puts("JNZ");
            break;
        case 0b0101:
            puts("RET");
            // RET
            break;
        case 0b0110:
            // ARIT
            puts("ARIT");
            break;
        case 0b1111:
            // HALT
            puts("HALT");
            break;

        default:
            break;
        }
        pc++;
        if (pc >= memSize)
            pc = 0;
    } while ((ri & 0xF000) != 0xF000);
}