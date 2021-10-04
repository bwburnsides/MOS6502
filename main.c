#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <inttypes.h>
#include "mos6502.h"


uint8_t C64_BusRead(uint16_t addr);
void C64_BusWrite(uint16_t addr, uint8_t data);

int main(int argc, char **argv)
{
    MOS6502 *cpu;

    cpu = mos6502(C64_BusRead, C64_BusWrite);
    if (cpu == NULL)
        return -1;

    free(cpu);
    return 0;
}

uint8_t C64_BusRead(uint16_t addr)
{
    return 0;
}

void C64_BusWrite(uint16_t addr, uint8_t data)
{

}
