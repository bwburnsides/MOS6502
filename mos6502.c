#include <stdlib.h>
#include "mos6502.h"
#include "mos6502_opcodes.h"

#define Inst(opcode, amode, func) cpu->instructions[opcode].mode = &amode; cpu->instructions[opcode].ucode = &func

MOS6502 *mos6502(MemRead read, MemWrite write)
{
    MOS6502 *cpu = malloc(sizeof(MOS6502));
    if (cpu == NULL)
        return cpu;

    cpu->read = read;
    cpu->write = write;

    _mos6502_build_instructions(cpu);
    return cpu;
}

void mos6502_run(MOS6502 *cpu, uint64_t cycles)
{
    Instruction inst;
    uint16_t src;

    while (cpu->cycles < cycles)
    {
        // Fetch
        inst = cpu->instructions[cpu->read(cpu->PC++)];

        // Decode
        src = inst.mode(cpu);

        // Execute
        inst.ucode(cpu, src);
    }
}

void _mos6502_build_instructions(MOS6502 *cpu)
{
    for (int i = 0; i < 256; i++)
    {
        cpu->instructions[i].mode = &mos6502_ImpMode;
        cpu->instructions[i].ucode = &mos6502_NOP;
    }

    Inst(ADC_IMM,      mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_ZPG,      mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_ZPG_XIDX, mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_ABS,      mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_ABS_XIDX, mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_ABS_YIDX, mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_XIDX_IND, mos6502_ImmMode, mos6502_ADC);
    Inst(ADC_IND_YIDX, mos6502_ImmMode, mos6502_ADC);

}

uint16_t mos6502_AccMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_AbsMode(MOS6502 *cpu)
{
    uint16_t addrL;
    uint16_t addrH;
    uint16_t addr;

    addrL = cpu->read(cpu->PC++);
    addrH = cpu->read(cpu->PC++);

    addr = (addrH << 8) + addrL;

    return addr;
}

uint16_t mos6502_ImmMode(MOS6502 *cpu)
{
    return cpu->PC++;
}

uint16_t mos6502_ImpMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_IndMode(MOS6502 *cpu)
{
    uint16_t addrL = cpu->read(cpu->PC++);
    uint16_t addrH = cpu->read(cpu->PC++);
    uint16_t ind_addr = (addrH << 8) + addrL;

    return cpu->read(ind_addr);
}

uint16_t mos6502_RelMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_ZpgMode(MOS6502 *cpu)
{
    uint8_t addrL = cpu->read(cpu->PC++);

    return ZPG_H + addrL;
}

uint16_t mos6502_XIdxIndMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_IndYIdxMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_AbsXIdxMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_AbsYIdxMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_ZpgXIdxMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_ZpgYIdxMode(MOS6502 *cpu)
{
    return 0;
}


void mos6502_reset(MOS6502 *cpu)
{
    cpu->A = cpu->X = cpu->Y = 0;
    cpu->SP = 0xFF;
    cpu->P = B_Mask | I_Mask;
    cpu->PC = (cpu->read(RST_VectorH) << 8) + cpu->read(RST_VectorL);

    cpu->cycles = 7;
}

void mos6502_nmi(MOS6502 *cpu, uint8_t state)
{
    // Push PC
    cpu->write(SP_H + cpu->SP, cpu->PC >> 8);
    cpu->SP--;
    cpu->write(SP_H + cpu->SP, (cpu->PC << 8) >> 8);
    cpu->SP--;

    // Push Processor Status Register
    cpu->write(SP_H + cpu->SP, cpu->P);
    cpu->SP--;

    // Read reset vector
    cpu->PC = (cpu->read(NMI_VectorH) << 8) + cpu->read(NMI_VectorL);

    cpu->cycles += 5;
}

void mos6502_irq(MOS6502 *cpu, uint8_t state)
{
    if (cpu->P & I_Mask)
    {
        // Push PC
        cpu->write(SP_H + cpu->SP, cpu->PC >> 8);
        cpu->SP--;
        cpu->write(SP_H + cpu->SP, (cpu->PC << 8) >> 8);
        cpu->SP--;

        // Push Processor Status Register
        cpu->write(SP_H + cpu->SP, cpu->P);
        cpu->SP--;

        // Read reset vector
        cpu->PC = (cpu->read(RST_VectorH) << 8) + cpu->read(RST_VectorL);

        cpu->cycles += 5;
    }
}

void mos6502_ADC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_AND(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_ASL(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BCC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BCS(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BEQ(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BIT(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BMI(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BNE(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BPL(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BRK(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BVC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BVS(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CLC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CLD(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CLI(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CLV(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CMP(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CPX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_CPY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_DEC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_DEX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_DEY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_EOR(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_INC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_INX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_INY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_JMP(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_JSR(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_LDA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_LDX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_LDY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_LSR(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_NOP(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_ORA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_PHA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_PHP(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_PLA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_PLP(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_ROL(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_ROR(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_RTI(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_RTS(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_SBC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_SEC(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_SED(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_SEI(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_STA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_STX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_STY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TAX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TAY(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TSX(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TXA(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TXS(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_TYA(MOS6502 *cpu, uint16_t src)
{

}
