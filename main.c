#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "mos6502.h"

#define RED_TXT "\033[0;31m"
#define GREEN_TXT "\033[0;32m"
#define RESET_TXT "\033[0m"

#define MEM_IMAGE "fib.bin"
#define MAX_INSTR_COUNT 100000

#define ADDR_SPACE_SIZE 64 * 1024
uint8_t memory[ADDR_SPACE_SIZE];

uint8_t DebugBusRead(uint16_t addr);
void DebugBusWrite(uint16_t addr, uint8_t data);
void print_cpu(MOS6502 *cpu);

#define STATE_FMT "|---------------------------------------------------------------------|\n"\
                  "| Instructions Executed: %29d | Code | Stack |\n"\
                  "|------------------------------------------------------|------|-------|\n"\
                  "| Status: %sN%s %sV%s - %sB%s %sD%s %sI%s %sZ%s %sC%s                              |      |       |\n"\
                  "| PC:    $%04X                                         |      |       |\n"\
                  "| SP:    $%04X                                         |      |       |\n"\
                  "| A:     $%02X                                           |      |       |\n"\
                  "| X:     $%02X                                           |      |       |\n"\
                  "| Y:     $%02X                                           |      |       |\n"\
                  "|------------------------------------------------------|      |       |\n"\
                  "| Zeropage:                                            |      |       |\n"\
                  "%s"\
                  "|------------------------------------------------------|------|-------|\n"

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

    for (int i = 0; i < MAX_INSTR_COUNT && !(cpu_run_rv); i++)
    {
        if (i)
            printf("\033[0;0H");

        cpu_run_rv = mos6502_run(cpu, 1);
        print_cpu(cpu);
    }

    free(cpu);
    return 0;
}

void print_cpu(MOS6502 *cpu)
{
    char zpg_str[913];
    int cursor;
    for (int i = 0; i < 16; i++)
    {
        cursor = i * 57;
        cursor += sprintf(zpg_str + cursor, "| $%02X:", i * 16);

        for (int j = 0; j < 16; j++)
        {
            cursor += sprintf(zpg_str + cursor, " %02X", memory[(ZPG_H + (i * 16)) + j]);
        }

        sprintf(zpg_str + cursor, " |\n");
    }

    printf(
        STATE_FMT,
        cpu->instr_cycles,
        IS_N ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_V ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_B ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_D ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_I ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_Z ? GREEN_TXT : RED_TXT, RESET_TXT,
        IS_C ? GREEN_TXT : RED_TXT, RESET_TXT,
        cpu->PC,
        SP_H + cpu->SP,
        cpu->A,
        cpu->X,
        cpu->Y,
        zpg_str
    );
}

uint8_t DebugBusRead(uint16_t addr)
{
    return memory[addr];
}

void DebugBusWrite(uint16_t addr, uint8_t data)
{
    memory[addr] = data;
}
