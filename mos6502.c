#define _CRT_SECURE_NO_DEPRECATE  // MSVC won't compile fopen or fscanf usage without
#include <stdlib.h>
#include <stdio.h>
#include "mos6502.h"

#define Inst(opcode, amode, func, name) instructions[(opcode)].mode = &(amode);\
                                        instructions[(opcode)].ucode = &(func);\
                                        instructions[(opcode)].mnemonic = (name);

int _INITIALIZED = 0;

MOS6502 *mos6502(MemRead read, MemWrite write)
{
    MOS6502 *cpu = malloc(sizeof(MOS6502));
    if (cpu == NULL)
        return cpu;

    cpu->read = read;
    cpu->write = write;
    cpu->instr_cycles = 0;
    cpu->IR = NOP_IMP;

    _mos6502_build_instructions();
    return cpu;
}

int mos6502_run(MOS6502 *cpu, uint64_t cycles)
{
    Instruction inst;
    uint16_t src;
    uint64_t initial_cycles = cpu->instr_cycles;

    while (cpu->instr_cycles < (initial_cycles + cycles))
    {
        // Fetch
        cpu->IR = cpu->read(cpu->PC);
        cpu->PC++;

        if (cpu->IR == HLT_IMP)
            return -1;

        inst = instructions[cpu->IR];

        // Decode
        src = inst.mode(cpu);

        // Execute
        inst.ucode(cpu, src);

        // Instruction complete
        cpu->instr_cycles++;
    }

    return 0;
}

void _mos6502_build_instructions()
{
    if (_INITIALIZED)
        return;

    // Fill Instruction table with NOPs
    for (int i = 0; i < 256; i++)
    {
        instructions[i].mode = &mos6502_ImpMode;
        instructions[i].ucode = &mos6502_NOP;
        instructions[i].mnemonic = "NOP";
    }

    // Fill actual instructions into table

    Inst(ADC_IMM,      mos6502_ImmMode,     mos6502_ADC, "ADC");
    Inst(ADC_ZPG,      mos6502_ZpgMode,     mos6502_ADC, "ADC");
    Inst(ADC_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_ADC, "ADC");
    Inst(ADC_ABS,      mos6502_AbsMode,     mos6502_ADC, "ADC");
    Inst(ADC_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_ADC, "ADC");
    Inst(ADC_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_ADC, "ADC");
    Inst(ADC_XIDX_IND, mos6502_XIdxIndMode, mos6502_ADC, "ADC");
    Inst(ADC_IND_YIDX, mos6502_IndYIdxMode, mos6502_ADC, "ADC");

    Inst(AND_IMM,      mos6502_ImmMode,     mos6502_AND, "AND");
    Inst(AND_ZPG,      mos6502_ZpgMode,     mos6502_AND, "AND");
    Inst(AND_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_AND, "AND");
    Inst(AND_ABS,      mos6502_AbsMode,     mos6502_AND, "AND");
    Inst(AND_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_AND, "AND");
    Inst(AND_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_AND, "AND");
    Inst(AND_XIDX_IND, mos6502_XIdxIndMode, mos6502_AND, "AND");
    Inst(AND_IND_YIDX, mos6502_IndYIdxMode, mos6502_AND, "AND");

    Inst(ASL_ACC,      mos6502_AccMode,     mos6502_ASL_AccMode, "ASL");
    Inst(ASL_ZPG,      mos6502_ZpgMode,     mos6502_ASL, "ASL");
    Inst(ASL_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_ASL, "ASL");
    Inst(ASL_ABS,      mos6502_AbsMode,     mos6502_ASL, "ASL");
    Inst(ASL_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_ASL, "ASL");

    Inst(BCC_REL,      mos6502_RelMode,     mos6502_BCC, "BCC");

    Inst(BCS_REL,      mos6502_RelMode,     mos6502_BCS, "BCS");

    Inst(BEQ_REL,      mos6502_RelMode,     mos6502_BEQ, "BEQ");

    Inst(BIT_ZPG,      mos6502_ZpgMode,     mos6502_BIT, "BIT");
    Inst(BIT_ABS,      mos6502_AbsMode,     mos6502_BIT, "BIT");

    Inst(BMI_REL,      mos6502_RelMode,     mos6502_BMI, "BMI");

    Inst(BNE_REL,      mos6502_RelMode,     mos6502_BNE, "BNE");

    Inst(BPL_REL,      mos6502_RelMode,     mos6502_BPL, "BPL");

    Inst(BRK_IMP,      mos6502_ImpMode,     mos6502_BRK, "BRK");

    Inst(BVC_REL,      mos6502_RelMode,     mos6502_BVC, "BVC");

    Inst(BVS_REL,      mos6502_RelMode,     mos6502_BVS, "BVS");

    Inst(CLC_IMP,      mos6502_ImpMode,     mos6502_CLC, "CLC");

    Inst(CLD_IMP,      mos6502_ImpMode,     mos6502_CLD, "CLD");

    Inst(CLI_IMP,      mos6502_ImpMode,     mos6502_CLI, "CLI");

    Inst(CLV_IMP,      mos6502_ImpMode,     mos6502_CLV, "CLV");

    Inst(CMP_IMM,      mos6502_ImmMode,     mos6502_CMP, "CMP");
    Inst(CMP_ZPG,      mos6502_ZpgMode,     mos6502_CMP, "CMP");
    Inst(CMP_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_CMP, "CMP");
    Inst(CMP_ABS,      mos6502_AbsMode,     mos6502_CMP, "CMP");
    Inst(CMP_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_CMP, "CMP");
    Inst(CMP_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_CMP, "CMP");
    Inst(CMP_XIDX_IND, mos6502_XIdxIndMode, mos6502_CMP, "CMP");
    Inst(CMP_IND_YIDX, mos6502_IndYIdxMode, mos6502_CMP, "CMP");

    Inst(CPX_IMM,      mos6502_ImmMode,     mos6502_CPX, "CPX");
    Inst(CPX_ZPG,      mos6502_ZpgMode,     mos6502_CPX, "CPX");
    Inst(CPX_ABS,      mos6502_AbsMode,     mos6502_CPX, "CPX");

    Inst(CPY_IMM,      mos6502_ImmMode,     mos6502_CPY, "CPY");
    Inst(CPY_ZPG,      mos6502_ZpgMode,     mos6502_CPY, "CPY");
    Inst(CPY_ABS,      mos6502_AbsMode,     mos6502_CPY, "CPY");

    Inst(DEC_ZPG,      mos6502_ZpgMode,     mos6502_DEC, "DEC");
    Inst(DEC_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_DEC, "DEC");
    Inst(DEC_ABS,      mos6502_AbsMode,     mos6502_DEC, "DEC");
    Inst(DEC_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_DEC, "DEC");

    Inst(DEX_IMP,      mos6502_ImpMode,     mos6502_DEX, "DEX");

    Inst(DEY_IMP,      mos6502_ImpMode,     mos6502_DEY, "DEY");

    Inst(EOR_IMM,      mos6502_ImmMode,     mos6502_EOR, "EOR");
    Inst(EOR_ZPG,      mos6502_ZpgMode,     mos6502_EOR, "EOR");
    Inst(EOR_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_EOR, "EOR");
    Inst(EOR_ABS,      mos6502_AbsMode,     mos6502_EOR, "EOR");
    Inst(EOR_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_EOR, "EOR");
    Inst(EOR_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_EOR, "EOR");
    Inst(EOR_XIDX_IND, mos6502_XIdxIndMode, mos6502_EOR, "EOR");
    Inst(EOR_IND_YIDX, mos6502_IndYIdxMode, mos6502_EOR, "EOR");

    Inst(INC_ZPG,      mos6502_ZpgMode,     mos6502_INC, "INC");
    Inst(INC_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_INC, "INC");
    Inst(INC_ABS,      mos6502_AbsMode,     mos6502_INC, "INC");
    Inst(INC_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_INC, "INC");

    Inst(INX_IMP,      mos6502_ImpMode,     mos6502_INX, "INX");

    Inst(INY_IMP,      mos6502_ImpMode,     mos6502_INY, "INY");

    Inst(JMP_ABS,      mos6502_AbsMode,     mos6502_JMP, "JMP");
    Inst(JMP_IND,      mos6502_IndMode,     mos6502_JMP, "JMP");

    Inst(JSR_ABS,      mos6502_AbsMode,     mos6502_JSR, "JSR");

    Inst(LDA_IMM,      mos6502_ImmMode,     mos6502_LDA, "LDA");
    Inst(LDA_ZPG,      mos6502_ZpgMode,     mos6502_LDA, "LDA");
    Inst(LDA_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_LDA, "LDA");
    Inst(LDA_ABS,      mos6502_AbsMode,     mos6502_LDA, "LDA");
    Inst(LDA_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_LDA, "LDA");
    Inst(LDA_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_LDA, "LDA");
    Inst(LDA_XIDX_IND, mos6502_XIdxIndMode, mos6502_LDA, "LDA");
    Inst(LDA_IND_YIDX, mos6502_IndYIdxMode, mos6502_LDA, "LDA");

    Inst(LDX_IMM,      mos6502_ImmMode,     mos6502_LDX, "LDX");
    Inst(LDX_ZPG,      mos6502_ZpgMode,     mos6502_LDX, "LDX");
    Inst(LDX_ZPG_YIDX, mos6502_ZpgYIdxMode, mos6502_LDX, "LDX");
    Inst(LDX_ABS,      mos6502_AbsMode,     mos6502_LDX, "LDX");
    Inst(LDX_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_LDX, "LDX");

    Inst(LDY_IMM,      mos6502_ImmMode,     mos6502_LDY, "LDY");
    Inst(LDY_ZPG,      mos6502_ZpgMode,     mos6502_LDY, "LDY");
    Inst(LDY_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_LDY, "LDY");
    Inst(LDY_ABS,      mos6502_AbsMode,     mos6502_LDY, "LDY");
    Inst(LDY_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_LDY, "LDY");

    Inst(LSR_ACC,      mos6502_AccMode,     mos6502_LSR_AccMode, "LSR");
    Inst(LSR_ZPG,      mos6502_ZpgMode,     mos6502_LSR, "LSR");
    Inst(LSR_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_LSR, "LSR");
    Inst(LSR_ABS,      mos6502_AbsMode,     mos6502_LSR, "LSR");
    Inst(LSR_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_LSR, "LSR");

    Inst(NOP_IMP,      mos6502_ImpMode,     mos6502_NOP, "NOP");

    Inst(ORA_IMM,      mos6502_ImmMode,     mos6502_ORA, "ORA");
    Inst(ORA_ZPG,      mos6502_ZpgMode,     mos6502_ORA, "ORA");
    Inst(ORA_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_ORA, "ORA");
    Inst(ORA_ABS,      mos6502_AbsMode,     mos6502_ORA, "ORA");
    Inst(ORA_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_ORA, "ORA");
    Inst(ORA_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_ORA, "ORA");
    Inst(ORA_XIDX_IND, mos6502_XIdxIndMode, mos6502_ORA, "ORA");
    Inst(ORA_IND_YIDX, mos6502_IndYIdxMode, mos6502_ORA, "ORA");

    Inst(PHA_IMP,      mos6502_ImpMode,     mos6502_PHA, "PHA");

    Inst(PHP_IMP,      mos6502_ImpMode,     mos6502_PHP, "PHP");

    Inst(PLA_IMP,      mos6502_ImpMode,     mos6502_PLA, "PLA");

    Inst(PLP_IMP,      mos6502_ImpMode,     mos6502_PLP, "PLP");

    Inst(ROL_ACC,      mos6502_AccMode,     mos6502_ROL_AccMode, "ROL");
    Inst(ROL_ZPG,      mos6502_ZpgMode,     mos6502_ROL, "ROL");
    Inst(ROL_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_ROL, "ROL");
    Inst(ROL_ABS,      mos6502_AbsMode,     mos6502_ROL, "ROL");
    Inst(ROL_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_ROL, "ROL");

    Inst(ROR_ACC,      mos6502_AccMode,     mos6502_ROR_AccMode, "ROR");
    Inst(ROR_ZPG,      mos6502_ZpgMode,     mos6502_ROR, "ROR");
    Inst(ROR_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_ROR, "ROR");
    Inst(ROR_ABS,      mos6502_AbsMode,     mos6502_ROR, "ROR");
    Inst(ROR_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_ROR, "ROR");

    Inst(RTI_IMP,      mos6502_ImpMode,     mos6502_RTI, "RTI");

    Inst(RTS_IMP,      mos6502_ImpMode,     mos6502_RTS, "RTS");

    Inst(SBC_IMM,      mos6502_ImmMode,     mos6502_SBC, "SBC");
    Inst(SBC_ZPG,      mos6502_ZpgMode,     mos6502_SBC, "SBC");
    Inst(SBC_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_SBC, "SBC");
    Inst(SBC_ABS,      mos6502_AbsMode,     mos6502_SBC, "SBC");
    Inst(SBC_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_SBC, "SBC");
    Inst(SBC_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_SBC, "SBC");
    Inst(SBC_XIDX_IND, mos6502_XIdxIndMode, mos6502_SBC, "SBC");
    Inst(SBC_IND_YIDX, mos6502_IndYIdxMode, mos6502_SBC, "SBC");

    Inst(SEC_IMP,      mos6502_ImpMode,     mos6502_SEC, "SEC");

    Inst(SED_IMP,      mos6502_ImpMode,     mos6502_SED, "SED");

    Inst(SEI_IMP,      mos6502_ImpMode,     mos6502_SEI, "SEI");

    Inst(STA_ZPG,      mos6502_ZpgMode,     mos6502_STA, "STA");
    Inst(STA_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_STA, "STA");
    Inst(STA_ABS,      mos6502_AbsMode,     mos6502_STA, "STA");
    Inst(STA_ABS_XIDX, mos6502_AbsXIdxMode, mos6502_STA, "STA");
    Inst(STA_ABS_YIDX, mos6502_AbsYIdxMode, mos6502_STA, "STA");
    Inst(STA_XIDX_IND, mos6502_XIdxIndMode, mos6502_STA, "STA");
    Inst(STA_IND_YIDX, mos6502_IndYIdxMode, mos6502_STA, "STA");

    Inst(STX_ZPG,      mos6502_ZpgMode,     mos6502_STX, "STX");
    Inst(STX_ZPG_YIDX, mos6502_ZpgYIdxMode, mos6502_STX, "STX");
    Inst(STX_ABS,      mos6502_AbsMode,     mos6502_STX, "STX");

    Inst(STY_ZPG,      mos6502_ZpgMode,     mos6502_STY, "STY");
    Inst(STY_ZPG_XIDX, mos6502_ZpgXIdxMode, mos6502_STY, "STY");
    Inst(STY_ABS,      mos6502_AbsMode,     mos6502_STY, "STY");

    Inst(TAX_IMP,      mos6502_ImpMode,     mos6502_TAX, "TAX");

    Inst(TAY_IMP,      mos6502_ImpMode,     mos6502_TAY, "TAY");

    Inst(TSX_IMP,      mos6502_ImpMode,     mos6502_TSX, "TSX");

    Inst(TXA_IMP,      mos6502_ImpMode,     mos6502_TXA, "TXA");

    Inst(TXS_IMP,      mos6502_ImpMode,     mos6502_TXS, "TXS");

    Inst(TYA_IMP,      mos6502_ImpMode,     mos6502_TYA, "TYA");

    Inst(HLT_IMP,      mos6502_ImpMode,     mos6502_NOP, "HLT");

    _INITIALIZED = 1;
}

uint16_t mos6502_AccMode(MOS6502 *cpu)
{
    return 0;  // value is unused
}

uint16_t mos6502_AbsMode(MOS6502 *cpu)
{
    // JMP $4032
    // 4C 32 40
    //  |  |  |
    //  |  |  -- Addr H
    //  |  ----- Addr L
    //  -------- Opcode
    //     ^
    //     |
    //     ----- PC
    // Return $4032
   
    uint16_t addrL = cpu->read(cpu->PC++);
    uint16_t addrH = cpu->read(cpu->PC++);

    return (addrH << 8) + addrL;
}

uint16_t mos6502_ImmMode(MOS6502 *cpu)
{
    // ORA #$B2
    // 09  B2
    //  |   |
    //  |   -- Imm
    //  ------ Opcode
    //      ^
    //      |
    //      -- PC
    // Return and increment PC
 
    return cpu->PC++;
}

uint16_t mos6502_ImpMode(MOS6502 *cpu)
{
    return 0;
}

uint16_t mos6502_IndMode(MOS6502 *cpu)
{
    // Memory:    
    // 1000 52 3A 04 D3 93 00 3F 93 84    

    // Instruction:    
    // JMP  ($1000) 
    // Return $3A52

    uint16_t addrL = cpu->read(cpu->PC++);
    uint16_t addrH = cpu->read(cpu->PC++);
    uint16_t addr = (addrH << 8) + addrL;

    return (cpu->read(addr + 1) << 8) + cpu->read(addr);
}

uint16_t mos6502_RelMode(MOS6502 *cpu)
{
    // BEQ #$B2
    // F0 B2
    //  |  |
    //  |  -- Offset
    //  ----- Opcode
    //     ^
    //     |
    //     -- PC
    // Return PC + Offset

    uint16_t offset = cpu->read(cpu->PC++);

	if (offset & 0x80)
        offset |= 0xFF00;

	return cpu->PC + (int16_t) offset;

}

uint16_t mos6502_ZpgMode(MOS6502 *cpu)
{
    // STA #$62
    // 85 62
    //  |  |
    //  |  -- ZPG Addr
    //  ----- Opcode
    //     ^
    //     |
    //     -- PC
    // Return ZPG_H + ZPG Addr

    return ZPG_H + cpu->read(cpu->PC++);
}

uint16_t mos6502_XIdxIndMode(MOS6502 *cpu)
{
    // LDA ($20, X)     X contains $04
    // 0024: 74 20
    // X + $20 = $24
    // Return Addr at $24 --> $2074

    // A1 20
    //  |  |
    //  |  -- Offset
    //  ----- Opcode
    //     ^
    //     |
    //     -- PC

    uint8_t offset = cpu->read(cpu->PC++);
    uint16_t ind_addr = ZPG_H + (cpu->X + offset);

    uint16_t addrL = cpu->read(ind_addr++);
    uint16_t addrH = cpu->read(ind_addr);

    return (addrH << 8) + addrL;
}

uint16_t mos6502_IndYIdxMode(MOS6502 *cpu)
{
    // LDA ($86),Y      Y = 10
    // B1 86
    //  |  |
    //  |  -- Offset
    //  ----- Opcode
    //     ^
    //     |
    //     -- PC

    // 0086: 28 40
    
    // 4028 + 10 = 4038
    // Return 4038

    uint16_t zpg_addr = ZPG_H | cpu->read(cpu->PC++);
    uint16_t ind_addrL = cpu->read(zpg_addr++); 
    uint16_t ind_addrH = cpu->read(zpg_addr++);

    return (ind_addrH << 8) + ind_addrL + cpu->Y;
}

uint16_t mos6502_AbsXIdxMode(MOS6502 *cpu)
{
	uint16_t addrL = cpu->read(cpu->PC++);
	uint16_t addrH = cpu->read(cpu->PC++);

	return (addrH << 8) + addrL + cpu->X;
}

uint16_t mos6502_AbsYIdxMode(MOS6502 *cpu)
{
	uint16_t addrL = cpu->read(cpu->PC++);
	uint16_t addrH = cpu->read(cpu->PC++);

	return addrL + (addrH << 8) + cpu->Y;
}

uint16_t mos6502_ZpgXIdxMode(MOS6502 *cpu)
{
    uint16_t addrL = cpu->read(cpu->PC++) + cpu->X;
    return ZPG_H + addrL;
}

uint16_t mos6502_ZpgYIdxMode(MOS6502 *cpu)
{
    uint16_t addrL = cpu->read(cpu->PC++) + cpu->Y;
    return ZPG_H + addrL;
}

void mos6502_rst(MOS6502 *cpu)
{
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;

    cpu->SP = 0xFF;
    cpu->P = B_Mask | I_Mask;
    cpu->PC = (cpu->read(RST_VectorH) << 8) + cpu->read(RST_VectorL);
}

void mos6502_nmi(MOS6502 *cpu, uint8_t state)
{
    // Set Break flag
    SET_B(0);

    // Push PC
    cpu->write(SP_H + cpu->SP, (cpu->PC >> 8) & 0xFF);
    cpu->SP--;

    cpu->write(SP_H + cpu->SP, cpu->PC & 0xFF);
    cpu->SP--;

    // Push Processor Status Register
    cpu->write(SP_H + cpu->SP, cpu->P);
    cpu->SP--;

    SET_I(1);

    // Read reset vector
    cpu->PC = (cpu->read(NMI_VectorH) << 8) + cpu->read(NMI_VectorL);
}

void mos6502_irq(MOS6502 *cpu, uint8_t state)
{
    if (IS_I)
    {
        // Set Break flag
        SET_B(0);

        // Push PC
        cpu->write(SP_H + cpu->SP, (cpu->PC >> 8) & 0xFF);
        cpu->SP--;

        cpu->write(SP_H + cpu->SP, cpu->PC & 0xFF);
        cpu->SP--;

        // Push Processor Status Register
        cpu->write(SP_H + cpu->SP, cpu->P);
        cpu->SP--;

        SET_I(1);

        // Read reset vector
        cpu->PC = (cpu->read(IRQ_VectorH) << 8) + cpu->read(IRQ_VectorL);
    }
}

void mos6502_ADC(MOS6502 *cpu, uint16_t src)
{
    // A + M + C -> A, C       N	Z	C	I	D	V
    //                         +	+	+	-	-	+

    uint8_t M = cpu->read(src);
    uint16_t sum = cpu->A + M + (IS_C ? 1 : 0);  // using 16-bit to have space for carry info

    SET_N(sum & N_Mask);
    SET_Z(!(sum & 0xFF));
    SET_V(!(((cpu->A ^ M) & N_Mask) && ((cpu->A ^ sum) & N_Mask)));
    SET_C(sum > 0xFF);

    cpu->A = sum & 0xFF;
}

void mos6502_AND(MOS6502 *cpu, uint16_t src)
{
    // A AND M -> A            N	Z	C	I	D	V
    //                         +	+	-	-	-	-

    uint8_t M = cpu->read(src);
    uint8_t result = cpu->A & M;

    SET_N(result & N_Mask);
    SET_Z(!(result & 0xFF));

    cpu->A = result;
}

void mos6502_ASL(MOS6502 *cpu, uint16_t src)
{
    // C <- [76543210] <- 0                N	Z	C	I	D	V
    //                                     +	+	+	-	-	-
    uint8_t M = cpu->read(src);

    SET_C(M & 0b10000000);
    M <<= 1;

    SET_N(M & 0b10000000);
    SET_Z(!M);

    cpu->write(src, M);
}

void mos6502_ASL_AccMode(MOS6502 *cpu, uint16_t src)
{
    // C <- [76543210] <- 0                N	Z	C	I	D	V
    //                                     +	+	+	-	-	-
    SET_C(cpu->A & 0b10000000);
    cpu->A <<= 1;

    SET_N(cpu->A & 0b10000000);
    SET_Z(!cpu->A);
}

void mos6502_BCC(MOS6502 *cpu, uint16_t src)
{
    if (!IS_C)
        cpu->PC = src;
}

void mos6502_BCS(MOS6502 *cpu, uint16_t src)
{
    if (IS_C)
        cpu->PC = src;
}

void mos6502_BEQ(MOS6502 *cpu, uint16_t src)
{
    if (IS_Z)
        cpu->PC = src;
}

void mos6502_BIT(MOS6502 *cpu, uint16_t src)
{
    //  AND M, M7 -> N, M6 -> V            N	Z	C	I	D	V
    //                                     M7	+	-	-	-	M6

    uint8_t M = cpu->read(src);
    uint8_t sum = cpu->A & M;

    SET_Z(!(sum & 0xFF));

    SET_N(M & 0b10000000);
    SET_V(M & 0b01000000);
}

void mos6502_BMI(MOS6502 *cpu, uint16_t src)
{
    if (IS_N)
        cpu->PC = src;
}

void mos6502_BNE(MOS6502 *cpu, uint16_t src)
{
    if (!IS_Z)
        cpu->PC = src;
}

void mos6502_BPL(MOS6502 *cpu, uint16_t src)
{
    if (!IS_N)
        cpu->PC = src;
}

void mos6502_BRK(MOS6502 *cpu, uint16_t src)
{

}

void mos6502_BVC(MOS6502 *cpu, uint16_t src)
{
    if (!IS_V)
        cpu->PC = src;
}

void mos6502_BVS(MOS6502 *cpu, uint16_t src)
{
    if (IS_V)
        cpu->PC = src;
}

void mos6502_CLC(MOS6502 *cpu, uint16_t src)
{
    SET_C(0);
}

void mos6502_CLD(MOS6502 *cpu, uint16_t src)
{
    SET_D(0);
}

void mos6502_CLI(MOS6502 *cpu, uint16_t src)
{
    SET_I(0);
}

void mos6502_CLV(MOS6502 *cpu, uint16_t src)
{
    SET_V(0);
}

void mos6502_CMP(MOS6502 *cpu, uint16_t src)
{
    // A - M           N	Z	C	I	D	V
    //                 +	+	+	-	-	-

    uint8_t M = cpu->read(src);
    uint16_t result = cpu->A - M;

    SET_N(result & 0b10000000);
    SET_Z(!(result & 0xFF));
    SET_C(result < 0x100);
}

void mos6502_CPX(MOS6502 *cpu, uint16_t src)
{
    // X - M               N	Z	C	I	D	V
    //                     +	+	+	-	-	-

    uint8_t M = cpu->read(src);
    uint16_t result = cpu->X - M;

    SET_N(result & 0b10000000);
    SET_Z(!(result & 0xFF));
    SET_C(result < 0x100);
}

void mos6502_CPY(MOS6502 *cpu, uint16_t src)
{
    // Y - M               N	Z	C	I	D	V
    //                     +	+	+	-	-	-

    uint8_t M = cpu->read(src);
    uint16_t result = cpu->Y - M;

    SET_N(result & 0b10000000);
    SET_Z(!(result & 0xFF));
    SET_C(result < 0x100);
}

void mos6502_DEC(MOS6502 *cpu, uint16_t src)
{
    // M - 1 -> M          N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    M--;
    SET_N(M & 0b10000000);
    SET_Z(!M);
    cpu->write(src, M);
}

void mos6502_DEX(MOS6502 *cpu, uint16_t src)
{
    // X - 1 -> X              N	Z	C	I	D	V
    //                         +	+	-	-	-	-
    cpu->X--;
    SET_N(cpu->X & 0b10000000);
    SET_Z(!cpu->X);
}

void mos6502_DEY(MOS6502 *cpu, uint16_t src)
{
    // Y - 1 -> Y              N	Z	C	I	D	V
    //                         +	+	-	-	-	-
    cpu->Y--;
    SET_N(cpu->Y & 0b10000000);
    SET_Z(!cpu->Y);
}

void mos6502_EOR(MOS6502 *cpu, uint16_t src)
{
    // A EOR M -> A                N	Z	C	I	D	V
    //                             +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    uint8_t result = cpu->A ^ M;

    SET_N(result & 0b10000000);
    SET_Z(!result);

    cpu->A = result;
}

void mos6502_INC(MOS6502 *cpu, uint16_t src)
{
    // M + 1 -> M              N	Z	C	I	D	V
    //                         +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    M++;

    SET_N(M & 0b10000000);
    SET_Z(!M);

    cpu->write(src, M);

}

void mos6502_INX(MOS6502 *cpu, uint16_t src)
{
    // X + 1 -> X              N	Z	C	I	D	V
    //                         +	+	-	-	-	-
    cpu->X++;

    SET_N(cpu->X & 0b10000000);
    SET_Z(!cpu->X);
}

void mos6502_INY(MOS6502 *cpu, uint16_t src)
{
    // Y + 1 -> Y              N	Z	C	I	D	V
    //                         +	+	-	-	-	-
    cpu->Y++;

    SET_N(cpu->Y & 0b10000000);
    SET_Z(!cpu->Y);
}

void mos6502_JMP(MOS6502 *cpu, uint16_t src)
{
    // (PC+1) -> PCL               N	Z	C	I	D	V
    // (PC+2) -> PCH               -	-	-	-	-	-
    cpu->PC = src;
}

void mos6502_JSR(MOS6502 *cpu, uint16_t src)
{
    // push (PC+2),            N	Z	C	I	D	V
    // (PC+1) -> PCL           -	-	-	-	-	-
    // (PC+2) -> PCH
    uint8_t pc_lo;
    uint8_t pc_hi;

    cpu->PC--;

    pc_lo = cpu->PC & 0xFF;
    pc_hi = (cpu->PC >> 8) & 0xFF;

    cpu->write(SP_H + cpu->SP, pc_hi);
    cpu->SP--;

    cpu->write(SP_H + cpu->SP, pc_lo);
    cpu->SP--;

    cpu->PC = src;

}

void mos6502_LDA(MOS6502 *cpu, uint16_t src)
{
    // M -> A              N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    SET_N(M & 0b10000000);
    SET_Z(!M);
    cpu->A = M;
}

void mos6502_LDX(MOS6502 *cpu, uint16_t src)
{
    // M -> X              N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    SET_N(M & 0b10000000);
    SET_Z(!M);
    cpu->X = M;
}

void mos6502_LDY(MOS6502 *cpu, uint16_t src)
{
    // M -> Y          N	Z	C	I	D	V
    //                 +	+	-	-	-	-
    uint8_t M = cpu->read(src);
    SET_N(M & 0b10000000);
    SET_Z(!M);
    cpu->Y = M;
}

void mos6502_LSR(MOS6502 *cpu, uint16_t src)
{
    // 0 -> [76543210] -> C            N	Z	C	I	D	V
    //                                 0	+	+	-	-	-
    uint8_t M = cpu->read(src);

    SET_C(M & 0b00000001);

    M >>= 1;

    SET_N(0);
    SET_Z(!M);

    cpu->write(src, M);
}

void mos6502_LSR_AccMode(MOS6502 *cpu, uint16_t src)
{
    // 0 -> [76543210] -> C            N	Z	C	I	D	V
    //                                 0	+	+	-	-	-

    SET_C(cpu->A & 0b00000001);

    cpu->A >>= 1;

    SET_N(0);
    SET_Z(!cpu->A);
}

void mos6502_NOP(MOS6502 *cpu, uint16_t src)
{
    // Do nothing
}

void mos6502_ORA(MOS6502 *cpu, uint16_t src)
{
    // A OR M -> A         N	Z	C	I	D	V
    //                     +	+	-	-	-	-

    uint8_t M = cpu->read(src);
    uint8_t result = cpu->A | M;

    SET_N(result & N_Mask);
    SET_Z(!(result & 0xFF));

    cpu->A = result;
}

void mos6502_PHA(MOS6502 *cpu, uint16_t src)
{
    // push A              N	Z	C	I	D	V
    //                     -	-	-	-	-	-
    cpu->write(SP_H + cpu->SP, cpu->A);
    cpu->SP--;
}

void mos6502_PHP(MOS6502 *cpu, uint16_t src)
{
    // The status register will be pushed with the break
    // flag and bit 5 set to 1.

    // push SR         N	Z	C	I	D	V
    //                 -	-	-	-	-	-

    uint8_t flags = cpu->P;

    flags |= B_Mask;
    flags |= BIT5_Mask;

    cpu->write(SP_H + cpu->SP, flags);
    cpu->SP--;
}

void mos6502_PLA(MOS6502 *cpu, uint16_t src)
{
    // pull A          N	Z	C	I	D	V
    //                 +	+	-	-	-	-

    cpu->SP++;
    cpu->A = cpu->read(SP_H + cpu->SP);

    SET_N(cpu->A & 0b10000000);
    SET_Z(!cpu->A);
}

void mos6502_PLP(MOS6502 *cpu, uint16_t src)
{
    // The status register will be pulled with the break
    // flag and bit 5 ignored.

    // pull SR             N	Z	C	I	D	V
    //                         from stack

    // TODO: respect the note about B and bit 5

    uint8_t old_P = cpu->P;

    cpu->SP++;
    cpu->P = cpu->read(SP_H + cpu->SP);

    (old_P & B_Mask) ? SET_B(1) : SET_B(0);
    (old_P & BIT5_Mask) ? SET_5(1) : SET_5(0);
}

void mos6502_ROL(MOS6502 *cpu, uint16_t src)
{
    // C <- [76543210] <- C            N	Z	C	I	D	V
    //                                 +	+	+	-	-	-

    uint16_t M = cpu->read(src);
    M <<= 1;

    if (IS_C)
        M |= 0b00000001;

    SET_C(M > 0xFF);
    M &= 0xFF;
    SET_N(M & 0b10000000);
    SET_Z(!M);

    cpu->write(src, (uint8_t) M);
}

void mos6502_ROL_AccMode(MOS6502 *cpu, uint16_t src)
{
    // C <- [76543210] <- C            N	Z	C	I	D	V
    //                                 +	+	+	-	-	-

    uint16_t M = cpu->A;
    M <<= 1;

    if (IS_C)
        M |= 0x00000001;

    SET_C(M > 0xFF);
    M &= 0xFF;
    SET_N(M & 0b10000000);
    SET_Z(!M);
    cpu->A = (uint8_t) M;
}

void mos6502_ROR(MOS6502 *cpu, uint16_t src)
{
    // C -> [76543210] -> C            N	Z	C	I	D	V
    //                                 +	+	+	-	-	-

    // Read value and shift it into high byte of M
    uint16_t M = cpu->read(src) << 8;

    // Shift right one place, now the Carry Out is in the high bit of low byte
    // And high bit of high byte is 1
    M >>= 1;

    // Set the high bit of the top byte if Carry In
    if (IS_C)
        M |= (0b10000000 << 8);

    // Use high bit of low byte to set new Carry Out
    SET_C(M & 0b10000000);

    // Move shifted value back into low byte and zero out high byte
    M >>= 8;
    M &= 0xFF;

    SET_N(M & 0b10000000);
    SET_Z(!M);

    cpu->write(src, (uint8_t) M);
}

void mos6502_ROR_AccMode(MOS6502 *cpu, uint16_t src)
{
    // C -> [76543210] -> C            N	Z	C	I	D	V
    //                                 +	+	+	-	-	-

    // Read value and shift it into high byte of M
    uint16_t M = cpu->A << 8;

    // Shift right one place, now the Carry Out is in the high bit of low byte
    // And high bit of high byte is 1
    M >>= 1;

    // Set the high bit of the top byte if Carry In
    if (IS_C)
        M |= (0b10000000 << 8);

    // Use high bit of low byte to set new Carry Out
    SET_C(M & 0b10000000);

    // Move shifted value back into low byte and zero out high byte
    M >>= 8;
    M &= 0xFF;

    SET_N(M & 0b10000000);
    SET_Z(!M);

    cpu->A = (uint8_t) M;
}

void mos6502_RTI(MOS6502 *cpu, uint16_t src)
{
    // The status register is pulled with the break flag
    // and bit 5 ignored. Then PC is pulled from the stack.

    // pull SR, pull PC         N	Z	C	I	D	V
    //                              from stack

    uint8_t pc_lo;
    uint8_t pc_hi;

    cpu->SP++;
    cpu->P = cpu->read(SP_H + cpu->SP);

    cpu->SP++;
    pc_lo = cpu->read(SP_H + cpu->SP);

    cpu->SP++;
    pc_hi = cpu->read(SP_H + cpu->SP);

    cpu->PC = (pc_hi << 8) | pc_lo;
}

void mos6502_RTS(MOS6502 *cpu, uint16_t src)
{
    // pull PC, PC+1 -> PC         N	Z	C	I	D	V
    //                             -	-	-	-	-	-

    uint8_t pc_lo;
    uint8_t pc_hi;

    cpu->SP++;
    pc_lo = cpu->read(SP_H + cpu->SP);

    cpu->SP++;
    pc_hi = cpu->read(SP_H + cpu->SP);

    cpu->PC = ((pc_hi << 8) | pc_lo) + 1;
}

void mos6502_SBC(MOS6502 *cpu, uint16_t src)
{
    // A - M - /C -> A          N	Z	C	I	D	V
    //                         +	+	+	-	-	+

    uint8_t M = cpu->read(src);
    uint16_t result = cpu->A - M - (IS_C ? 0 : 1);

    SET_N(result & 0b10000000);
    SET_Z(!(result & 0xFF));
    SET_C(result < 0x100);
    SET_V(!(((cpu->A ^ M) & 0b10000000) && ((cpu->A ^ result) & 0b10000000)));

    cpu->A = result & 0xFF;
}

void mos6502_SEC(MOS6502 *cpu, uint16_t src)
{
    SET_C(1);
}

void mos6502_SED(MOS6502 *cpu, uint16_t src)
{
    SET_D(1);
}

void mos6502_SEI(MOS6502 *cpu, uint16_t src)
{
    SET_I(1);
}

void mos6502_STA(MOS6502 *cpu, uint16_t src)
{
    cpu->write(src, cpu->A);
}

void mos6502_STX(MOS6502 *cpu, uint16_t src)
{
    cpu->write(src, cpu->X);
}

void mos6502_STY(MOS6502 *cpu, uint16_t src)
{
    cpu->write(src, cpu->Y);
}

void mos6502_TAX(MOS6502 *cpu, uint16_t src)
{
    // A -> X              N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    SET_N(cpu->A | 0b10000000);
    SET_Z(!cpu->A);
    cpu->X = cpu->A;
}

void mos6502_TAY(MOS6502 *cpu, uint16_t src)
{
    // A -> Y              N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    SET_N(cpu->A | 0b10000000);
    SET_Z(!cpu->A);
    cpu->Y = cpu->A;
}

void mos6502_TSX(MOS6502 *cpu, uint16_t src)
{
    // SP -> X             N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    SET_N(cpu->SP | 0b10000000);
    SET_Z(!cpu->SP);
    cpu->X = cpu->SP;
}

void mos6502_TXA(MOS6502 *cpu, uint16_t src)
{
    // X -> A              N	Z	C	I	D	V
    //                     +	+	-	-	-	-
    SET_N(cpu->X | 0b10000000);
    SET_Z(!cpu->X);
    cpu->A = cpu->X;
}

void mos6502_TXS(MOS6502 *cpu, uint16_t src)
{
    // X -> SP             N	Z	C	I	D	V
    //                     -	-	-	-	-	-
    cpu->SP = cpu->X;
}

void mos6502_TYA(MOS6502 *cpu, uint16_t src)
{
    // Y -> A          N	Z	C	I	D	V
    //                 +	+	-	-	-	-
    SET_N(cpu->Y | 0b10000000);
    SET_Z(!cpu->Y);
    cpu->A = cpu->Y;
}

void mos6502_disassemble(MOS6502 *cpu, uint16_t count, uint16_t start_addr, uint16_t **addrs, char ***assembly)
{
    uint16_t addr = start_addr;
    uint16_t lo;
    uint16_t hi;
    uint8_t opcode;
    uint8_t value;

    *assembly = calloc(count, sizeof(char*));
    if (*assembly == NULL)
        exit(-1);

    *addrs = calloc(count, sizeof(uint16_t));
    if (*addrs == NULL)
        exit(-1);

    for (int i = 0; i < count; i++)
    {
        (*assembly)[i] = (char *) calloc(14, sizeof(char));
        if ((*assembly)[i] == NULL)
            exit(-1);

        (*addrs)[i] = addr;
        opcode = cpu->read(addr++);

        if (instructions[opcode].mode == mos6502_ImpMode)
        {
            sprintf((*assembly)[i], "%s", instructions[opcode].mnemonic);
        }
        else if (instructions[opcode].mode == mos6502_ImmMode)
        {
            value = cpu->read(addr++);
            sprintf((*assembly)[i], "%s #$%02X", instructions[opcode].mnemonic, value);
        }
        else if (instructions[opcode].mode == mos6502_ZpgMode)
        {
            value = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%02X", instructions[opcode].mnemonic, value);
        }
        else if (instructions[opcode].mode == mos6502_ZpgXIdxMode)
        {
            value = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%02X, X", instructions[opcode].mnemonic, value);
        }
        else if (instructions[opcode].mode == mos6502_ZpgYIdxMode)
        {
            value = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%02X, Y", instructions[opcode].mnemonic, value);
        }
        else if (instructions[opcode].mode == mos6502_XIdxIndMode)
        {
            lo = cpu->read(addr++);
            sprintf((*assembly)[i], "%s ($%02X, X)", instructions[opcode].mnemonic, lo);
        }
        else if (instructions[opcode].mode == mos6502_IndYIdxMode)
        {
            lo = cpu->read(addr++);
            sprintf((*assembly)[i], "%s ($%02X), Y", instructions[opcode].mnemonic, lo);
        }
        else if (instructions[opcode].mode == mos6502_AbsMode)
        {
            lo = cpu->read(addr++);
            hi = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%04X", instructions[opcode].mnemonic, (uint16_t)(hi << 8) | lo);
        }
        else if (instructions[opcode].mode == mos6502_AbsXIdxMode)
        {
            lo = cpu->read(addr++);
            hi = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%04X, X", instructions[opcode].mnemonic, (uint16_t)(hi << 8) | lo);
        }
        else if (instructions[opcode].mode == mos6502_AbsYIdxMode)
        {
            lo = cpu->read(addr++);
            hi = cpu->read(addr++);
            sprintf((*assembly)[i], "%s $%04X, Y", instructions[opcode].mnemonic, (uint16_t)(hi << 8) | lo);
        }
        else if (instructions[opcode].mode == mos6502_IndMode)
        {
            lo = cpu->read(addr++);
            hi = cpu->read(addr++);
            sprintf((*assembly)[i], "%s ($%04X)", instructions[opcode].mnemonic, (uint16_t)(hi << 8) | lo);
        }
        else if (instructions[opcode].mode == mos6502_RelMode)
        {
            value = cpu->read(addr++);
            sprintf((*assembly)[i], "%s [$%02X]", instructions[opcode].mnemonic, value);
        }
    }
}
