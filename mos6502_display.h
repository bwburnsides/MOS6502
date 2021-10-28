#ifndef MOS6502_DISPLAY_H
#define MOS6502_DISPLAY_H

#include "mos6502.h"

#define RED_TXT "\033[0;31m"
#define GREEN_TXT "\033[0;32m"
#define RESET_TXT "\033[0m"
#define STATE_FMT "\033[1;1H"\
"+------------------------------------------------------+----------------------+-----------+\n"\
"| Instructions Executed: %29d | Program              | Stack     |\n"\
"+------------------------------------------------------+----------------------+-----------+\n"\
"| Status: %sN %sV \033[0m- %sB %sD %sI %sZ %sC%-32s  | %-20s | %-9s |\n"\
"| PC:    $%04X                                         | %-20s | %-9s |\n"\
"| SP:    $%04X                                         | %-20s | %-9s |\n"\
"| A:     $%02X                                           | %-20s | %-9s |\n"\
"| X:     $%02X                                           | %-20s | %-9s |\n"\
"| Y:     $%02X                                           | %-20s | %-9s |\n"\
"+------------------------------------------------------+ %-20s | %-9s |\n"\
"| Zeropage:                                            | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"| %s | %-20s | %-9s |\n"\
"+------------------------------------------------------+----------------------+-----------+\n"

void _mos6502_build_zpg_rows(MOS6502 *cpu, char ***rows);
void _mos6502_build_prog_rows(MOS6502 *cpu, char ***rows);
void _mos6502_build_stack_rows(MOS6502 *cpu, char ***rows);
void mos6502_print(MOS6502 *cpu);

#endif // MOS6502_DISPLAY_H