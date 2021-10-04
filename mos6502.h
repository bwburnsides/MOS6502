#include <inttypes.h>

static const uint8_t C_Mask = 0x01;
static const uint8_t Z_Mask = 0x02;
static const uint8_t I_Mask = 0x04;
static const uint8_t D_Mask = 0x08;
static const uint8_t B_Mask = 0x10;
static const uint8_t V_Mask = 0x40;
static const uint8_t N_Mask = 0x80;

static const uint16_t NMI_VectorL = 0xFFFA;
static const uint16_t NMI_VectorH = 0xFFFB;
static const uint16_t RST_VectorL = 0xFFFC;
static const uint16_t RST_VectorH = 0xFFFD;
static const uint16_t IRQ_VectorL = 0xFFFE;
static const uint16_t IRQ_VectorH = 0xFFFF;

static const uint16_t SP_H = 0x0100;
static const uint16_t ZPG_H = 0x0000;

typedef struct _MOS6502 MOS6502;
typedef uint8_t (* MemRead)(uint16_t);
typedef void (* MemWrite)(uint16_t, uint8_t);
typedef uint8_t (* AddrMode)(MOS6502 *cpu);
typedef void (* InstUcode)(MOS6502 *cpu, uint8_t operand);

struct Instruction
{
    AddrMode mode;
    InstUcode ucode;
};

struct _MOS6502
{
    uint64_t cycles;

    MemRead read;
    MemWrite write;

    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint8_t SP;
    uint16_t PC;

    uint8_t nmi;
    uint8_t irq;

    struct Instruction instructions[256];
};

MOS6502 *mos6502(MemRead read, MemWrite write);
void mos6502_reset(MOS6502 *cpu);
void mos6502_nmi(MOS6502 *cpu, uint8_t state);
void mos6502_irq(MOS6502 *cpu, uint8_t state);
void mos6502_run(MOS6502 *cpu, uint64_t cycles);

void _mos6502_build_instructions(MOS6502 *cpu);

uint16_t mos6502_AccMode(MOS6502 *cpu);
uint16_t mos6502_AbsMode(MOS6502 *cpu);
uint16_t mos6502_ImmMode(MOS6502 *cpu);
uint16_t mos6502_ImpMode(MOS6502 *cpu);
uint16_t mos6502_IndMode(MOS6502 *cpu);
uint16_t mos6502_RelMode(MOS6502 *cpu);
uint16_t mos6502_ZpgMode(MOS6502 *cpu);
uint16_t mos6502_XIdxIndMode(MOS6502 *cpu);
uint16_t mos6502_IndYIdxMode(MOS6502 *cpu);
uint16_t mos6502_AbsXIdxMode(MOS6502 *cpu);
uint16_t mos6502_AbsYIdxMode(MOS6502 *cpu);
uint16_t mos6502_ZpgXIdxMode(MOS6502 *cpu);
uint16_t mos6502_ZpgYIdxMode(MOS6502 *cpu);

void mos6502_ADC(MOS6502 *cpu, uint8_t operand);
void mos6502_AND(MOS6502 *cpu, uint8_t operand);
void mos6502_ASL(MOS6502 *cpu, uint8_t operand);
void mos6502_BCC(MOS6502 *cpu, uint8_t operand);
void mos6502_BCS(MOS6502 *cpu, uint8_t operand);
void mos6502_BEQ(MOS6502 *cpu, uint8_t operand);
void mos6502_BIT(MOS6502 *cpu, uint8_t operand);
void mos6502_BMI(MOS6502 *cpu, uint8_t operand);
void mos6502_BNE(MOS6502 *cpu, uint8_t operand);
void mos6502_BPL(MOS6502 *cpu, uint8_t operand);
void mos6502_BRK(MOS6502 *cpu, uint8_t operand);
void mos6502_BVC(MOS6502 *cpu, uint8_t operand);
void mos6502_BVS(MOS6502 *cpu, uint8_t operand);
void mos6502_CLC(MOS6502 *cpu, uint8_t operand);
void mos6502_CLD(MOS6502 *cpu, uint8_t operand);
void mos6502_CLI(MOS6502 *cpu, uint8_t operand);
void mos6502_CLV(MOS6502 *cpu, uint8_t operand);
void mos6502_CMP(MOS6502 *cpu, uint8_t operand);
void mos6502_CPX(MOS6502 *cpu, uint8_t operand);
void mos6502_CPY(MOS6502 *cpu, uint8_t operand);
void mos6502_DEC(MOS6502 *cpu, uint8_t operand);
void mos6502_DEX(MOS6502 *cpu, uint8_t operand);
void mos6502_DEY(MOS6502 *cpu, uint8_t operand);
void mos6502_EOR(MOS6502 *cpu, uint8_t operand);
void mos6502_INC(MOS6502 *cpu, uint8_t operand);
void mos6502_INX(MOS6502 *cpu, uint8_t operand);
void mos6502_INY(MOS6502 *cpu, uint8_t operand);
void mos6502_JMP(MOS6502 *cpu, uint8_t operand);
void mos6502_JSR(MOS6502 *cpu, uint8_t operand);
void mos6502_LDA(MOS6502 *cpu, uint8_t operand);
void mos6502_LDX(MOS6502 *cpu, uint8_t operand);
void mos6502_LDY(MOS6502 *cpu, uint8_t operand);
void mos6502_LSR(MOS6502 *cpu, uint8_t operand);
void mos6502_NOP(MOS6502 *cpu, uint8_t operand);
void mos6502_ORA(MOS6502 *cpu, uint8_t operand);
void mos6502_PHA(MOS6502 *cpu, uint8_t operand);
void mos6502_PHP(MOS6502 *cpu, uint8_t operand);
void mos6502_PLA(MOS6502 *cpu, uint8_t operand);
void mos6502_PLP(MOS6502 *cpu, uint8_t operand);
void mos6502_ROL(MOS6502 *cpu, uint8_t operand);
void mos6502_ROR(MOS6502 *cpu, uint8_t operand);
void mos6502_RTI(MOS6502 *cpu, uint8_t operand);
void mos6502_RTS(MOS6502 *cpu, uint8_t operand);
void mos6502_SBC(MOS6502 *cpu, uint8_t operand);
void mos6502_SEC(MOS6502 *cpu, uint8_t operand);
void mos6502_SED(MOS6502 *cpu, uint8_t operand);
void mos6502_SEI(MOS6502 *cpu, uint8_t operand);
void mos6502_STA(MOS6502 *cpu, uint8_t operand);
void mos6502_STX(MOS6502 *cpu, uint8_t operand);
void mos6502_STY(MOS6502 *cpu, uint8_t operand);
void mos6502_TAX(MOS6502 *cpu, uint8_t operand);
void mos6502_TAY(MOS6502 *cpu, uint8_t operand);
void mos6502_TSX(MOS6502 *cpu, uint8_t operand);
void mos6502_TXA(MOS6502 *cpu, uint8_t operand);
void mos6502_TXS(MOS6502 *cpu, uint8_t operand);
void mos6502_TYA(MOS6502 *cpu, uint8_t operand);
