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


int main(int argc, char **argv)
{
    MOS6502 *cpu;
    FILE *fp;
    size_t num_bytes;

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

    printf("----------------------------------\n");
    printf("Starting Emulator. Initial state of CPU:\n\n");
    print_cpu(cpu);

    // for (int i = 0; i < MAX_INSTR_COUNT; i++)
    // {
    //     mos6502_run(cpu, 1);
    //     print_cpu(cpu);
    //     getchar();
    // }

    mos6502_run(cpu, MAX_INSTR_COUNT);

    printf("\n----------------------------------\n");
    printf("\nFinal state of CPU:\n\n");
    print_cpu(cpu);

    free(cpu);
    return 0;
}

void print_cpu(MOS6502 *cpu)
{
    printf("Instructions: %d\n", cpu->instr_cycles);
    printf("PC: $%04X\t\tIR: $%02X\t\tSP: $%04X\n", cpu->PC, cpu->IR, SP_H + cpu->SP);
    printf("\nStatus: %d  %d  %d  %d  %d  %d  %d  %d\n", IS_N, IS_V, IS_5, IS_B, IS_D, IS_I, IS_Z, IS_C);
    printf("        N  V  -  B  D  I  Z  C\n");
    printf("A: $%02X\n", cpu->A);
    printf("X: $%02X\n", cpu->X);
    printf("Y: $%02X\n", cpu->Y);

    printf("\nZeropage:\n");
    for (int i = 0; i < 256; i += 16)
    {
        printf("$%02X: ", i);
        for (int j = 0; j < 16; j++)
            printf("%02X ", memory[(ZPG_H + i) + j]);
        printf("\n");
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
