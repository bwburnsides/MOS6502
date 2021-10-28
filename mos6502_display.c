#define _CRT_SECURE_NO_DEPRECATE  // MSVC won't compile fopen or fscanf usage without
#include <stdio.h>
#include <stdlib.h>
#include "mos6502_display.h"

void _mos6502_build_zpg_rows(MOS6502 *cpu, char ***rows)
{
    int cursor;

    *rows = calloc(16, sizeof(char*));
    if (*rows == NULL)
        exit(-1);

    for (int i = 0; i < 16; i++)
    {
        (*rows)[i] = calloc(53, sizeof(char));
        if ((*rows)[i] == NULL)
            exit(-1);

        cursor = 0;
        cursor += sprintf((*rows)[i] + cursor, "$%02X:", i * 16);

        for (int j = 0; j < 16; j++)
            cursor += sprintf((*rows)[i] + cursor, " %02X", cpu->read((ZPG_H + (i * 16)) + j));
    }
}

void _mos6502_build_prog_rows(MOS6502 *cpu, char ***rows)
{
    uint16_t *addrs;
    char **assembly;

    mos6502_disassemble(cpu, 24, cpu->PC - 9, &addrs, &assembly);

    *rows = calloc(42, sizeof(char*));
    if (*rows == NULL)
        exit(-1);

    for (int i = 0; i < 24; i++)
    {
        (*rows)[i] = calloc(30, sizeof(char));
        if ((*rows)[i] == NULL)
            exit(-1);

        sprintf((*rows)[i], "$%04X: %-13.13s", addrs[i], assembly[i]);
    }

    for (int i = 0; i < 24; i++)
        free(assembly[i]);

    free(addrs);
    free(assembly);
}

void _mos6502_build_stack_rows(MOS6502 *cpu, char ***rows)
{
    uint8_t addr;

    *rows = calloc(24, sizeof(char*));
    if (*rows == NULL)
        exit(-1);

    for (int i = 0; i < 24; i++)
    {
        (*rows)[i] = calloc(21, sizeof(char));
        if ((*rows)[i] == NULL)
            exit(-1);

        addr = cpu->SP + i - 7;
        sprintf((*rows)[i], "%s$%04X%s: %02X", addr == cpu->SP ? GREEN_TXT : RESET_TXT, SP_H | addr, RESET_TXT, cpu->read(SP_H | addr));
    }
}

void mos6502_print(MOS6502 *cpu)
{
    char **zpg_rows;
    char **prog_rows;
    char **stack_rows;

    _mos6502_build_zpg_rows(cpu, &zpg_rows);
    _mos6502_build_prog_rows(cpu, &prog_rows);
    _mos6502_build_stack_rows(cpu, &stack_rows);

    printf(
        STATE_FMT,
        cpu->instr_cycles,
        IS_N ? GREEN_TXT : RED_TXT,
        IS_V ? GREEN_TXT : RED_TXT,
        IS_B ? GREEN_TXT : RED_TXT,
        IS_D ? GREEN_TXT : RED_TXT,
        IS_I ? GREEN_TXT : RED_TXT,
        IS_Z ? GREEN_TXT : RED_TXT,
        IS_C ? GREEN_TXT : RED_TXT,
        RESET_TXT,
        prog_rows[0],
        stack_rows[0],
        cpu->PC,
        prog_rows[1],
        stack_rows[1],
        SP_H + cpu->SP,
        prog_rows[2],
        stack_rows[2],
        cpu->A,
        prog_rows[3],
        stack_rows[3],
        cpu->X,
        prog_rows[4],
        stack_rows[4],
        cpu->Y,
        prog_rows[5],
        stack_rows[5],
        prog_rows[6],
        stack_rows[6],
        prog_rows[7],
        stack_rows[7],
        zpg_rows[0],
        prog_rows[8],
        stack_rows[8],
        zpg_rows[1],
        prog_rows[9],
        stack_rows[9],
        zpg_rows[2],
        prog_rows[10],
        stack_rows[10],
        zpg_rows[3],
        prog_rows[11],
        stack_rows[11],
        zpg_rows[4],
        prog_rows[12],
        stack_rows[12],
        zpg_rows[5],
        prog_rows[13],
        stack_rows[13],
        zpg_rows[6],
        prog_rows[14],
        stack_rows[14],
        zpg_rows[7],
        prog_rows[15],
        stack_rows[15],
        zpg_rows[8],
        prog_rows[16],
        stack_rows[16],
        zpg_rows[9],
        prog_rows[17],
        stack_rows[17],
        zpg_rows[10],
        prog_rows[18],
        stack_rows[18],
        zpg_rows[11],
        prog_rows[19],
        stack_rows[19],
        zpg_rows[12],
        prog_rows[20],
        stack_rows[20],
        zpg_rows[13],
        prog_rows[21],
        stack_rows[21],
        zpg_rows[14],
        prog_rows[22],
        stack_rows[22],
        zpg_rows[15],
        prog_rows[23],
        stack_rows[23]
    );

    for (int i = 0; i < 16; i++)
        free(zpg_rows[i]);
    free(zpg_rows);

    for (int i = 0; i < 24; i++)
        free(prog_rows[i]);
    free(prog_rows);

    for (int i = 0; i < 24; i++)
        free(stack_rows[i]);
    free(stack_rows);
}
