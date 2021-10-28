#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "mos6502.h"
#include "mos6502_display.h"

#define MEM_IMAGE "fib.bin"
#define MAX_INSTR_COUNT 100000

#define ADDR_SPACE_SIZE 64 * 1024
uint8_t memory[ADDR_SPACE_SIZE];

void DebugBusInit(MOS6502 *cpu, char *mem_fname);
void DebugBusRun(MOS6502 *cpu, int max_instr_count);
uint8_t DebugBusRead(uint16_t addr);
void DebugBusWrite(uint16_t addr, uint8_t data);


int main(int argc, char **argv)
{
    MOS6502 *cpu;
    int cpu_run_rv = 0;

    cpu = mos6502(DebugBusRead, DebugBusWrite);
    if (cpu == NULL)
        return -1;

    DebugBusInit(cpu, MEM_IMAGE);
    DebugBusRun(cpu, MAX_INSTR_COUNT);

    free(cpu);
    return 0;
}

void DebugBusInit(MOS6502 *cpu, char *mem_fname)
{
    FILE *fp;
    size_t num_bytes;

    fp = fopen(mem_fname, "rb");
    if (fp == NULL)
        exit(-1);

    num_bytes = fread(memory, sizeof(uint8_t), ADDR_SPACE_SIZE, fp);
    fclose(fp);

    for (int i = num_bytes; i < ADDR_SPACE_SIZE; i++)
        memory[i] = NOP_IMP;
}

void DebugBusRun(MOS6502 *cpu, int max_instr_count)
{
    mos6502_rst(cpu);
    mos6502_print(cpu);

    for (int i = 0, cpu_run_rv = 0; i < max_instr_count && !(cpu_run_rv); i++)
    {
        cpu_run_rv = mos6502_run(cpu, 1);
        mos6502_print(cpu);
    }
}

uint8_t DebugBusRead(uint16_t addr)
{
    return memory[addr];
}

void DebugBusWrite(uint16_t addr, uint8_t data)
{
    memory[addr] = data;
}
