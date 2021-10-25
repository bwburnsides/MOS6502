#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "mos6502.h"

#define MEM_IMAGE "fib.bin"
#define MAX_INSTR_COUNT 100000

#define ADDR_SPACE_SIZE 64 * 1024
uint8_t memory[ADDR_SPACE_SIZE];

uint8_t DebugBusRead(uint16_t addr);
void DebugBusWrite(uint16_t addr, uint8_t data);
void print_cpu(MOS6502 *cpu);
void build_zpg_rows(MOS6502 *cpu, char ***rows);
void build_prog_rows(MOS6502 *cpu, char ***rows);
void build_stack_rows(MOS6502 *cpu, char ***rows);


int main(int argc, char **argv)
{
    MOS6502 *cpu;
    FILE *fp;
    size_t num_bytes;
    int cpu_run_rv = 0;

    cpu = mos6502(DebugBusRead, DebugBusWrite);
    if (cpu == NULL)
        return -1;

    fp = fopen(MEM_IMAGE, "rb");
    if (fp == NULL)
        return -1;

    num_bytes = fread(memory, sizeof(uint8_t), ADDR_SPACE_SIZE, fp);
    fclose(fp);

    for (int i = num_bytes; i < ADDR_SPACE_SIZE; i++)
        memory[i] = NOP_IMP;

    mos6502_rst(cpu);
    mos6502_print(cpu);

    for (int i = 0; i < MAX_INSTR_COUNT && !(cpu_run_rv); i++)
    {
        cpu_run_rv = mos6502_run(cpu, 1);
        mos6502_print(cpu);
    }

    free(cpu);
    return 0;
}

uint8_t DebugBusRead(uint16_t addr)
{
    return memory[addr];
}

void DebugBusWrite(uint16_t addr, uint8_t data)
{
    memory[addr] = data;
}
