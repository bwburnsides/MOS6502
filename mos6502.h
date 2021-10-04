#include <inttypes.h>

#define C_Mask ((uint8_t) 0x01)  // 0b00000001
#define Z_Mask ((uint8_t) 0x02)  // 0b00000010
#define I_Mask ((uint8_t) 0x04)  // 0b00000100
#define D_Mask ((uint8_t) 0x08)  // 0b00001000
#define B_Mask ((uint8_t) 0x10)  // 0b00010000
#define V_Mask ((uint8_t) 0x40)  // 0b01000000
#define N_Mask ((uint8_t) 0x80)  // 0b10000000

#define SET_C(x) ((x) ? (cpu->P |= C_Mask) : (cpu->P &= (~C_Mask)))
#define SET_Z(x) ((x) ? (cpu->P |= Z_Mask) : (cpu->P &= (~Z_Mask)))
#define SET_I(x) ((x) ? (cpu->P |= I_Mask) : (cpu->P &= (~I_Mask)))
#define SET_D(x) ((x) ? (cpu->P |= D_Mask) : (cpu->P &= (~D_Mask)))
#define SET_B(x) ((x) ? (cpu->P |= B_Mask) : (cpu->P &= (~B_Mask)))
#define SET_V(x) ((x) ? (cpu->P |= V_Mask) : (cpu->P &= (~V_Mask)))
#define SET_N(x) ((x) ? (cpu->P |= N_Mask) : (cpu->P &= (~N_Mask)))

#define IS_C ((cpu->P & C_Mask) ? 1 : 0)
#define IS_Z ((cpu->P & Z_Mask) ? 1 : 0)
#define IS_I ((cpu->P & I_Mask) ? 1 : 0)
#define IS_D ((cpu->P & D_Mask) ? 1 : 0)
#define IS_B ((cpu->P & B_Mask) ? 1 : 0)
#define IS_V ((cpu->P & V_Mask) ? 1 : 0)
#define IS_N ((cpu->P & N_Mask) ? 1 : 0)

#define NMI_VectorL  ((uint16_t) 0xFFFA)
#define NMI_VectorH  ((uint16_t) 0xFFFB)
#define RST_VectorL  ((uint16_t) 0xFFFC)
#define RST_VectorH  ((uint16_t) 0xFFFD)
#define IRQ_VectorL  ((uint16_t) 0xFFFE)
#define IRQ_VectorH  ((uint16_t) 0xFFFF)

#define SP_H ((uint16_t) 0x0100)
#define ZPG_H ((uint16_t) 0x0000)

typedef struct _MOS6502 MOS6502;
typedef uint8_t (* MemRead)(uint16_t addr);
typedef void (* MemWrite)(uint16_t addr, uint8_t data);
typedef uint16_t (* AddrMode)(MOS6502 *cpu);
typedef void (* InstUcode)(MOS6502 *cpu, uint16_t src);

typedef struct _Instruction
{
    AddrMode mode;
    InstUcode ucode;
} Instruction;

struct _MOS6502
{
    uint64_t instr_cycles;

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

    Instruction instructions[256];
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

void mos6502_ADC(MOS6502 *cpu, uint16_t src);
void mos6502_AND(MOS6502 *cpu, uint16_t src);
void mos6502_ASL(MOS6502 *cpu, uint16_t src);
void mos6502_ASL_AccMode(MOS6502 *cpu, uint16_t src);
void mos6502_BCC(MOS6502 *cpu, uint16_t src);
void mos6502_BCS(MOS6502 *cpu, uint16_t src);
void mos6502_BEQ(MOS6502 *cpu, uint16_t src);
void mos6502_BIT(MOS6502 *cpu, uint16_t src);
void mos6502_BMI(MOS6502 *cpu, uint16_t src);
void mos6502_BNE(MOS6502 *cpu, uint16_t src);
void mos6502_BPL(MOS6502 *cpu, uint16_t src);
void mos6502_BRK(MOS6502 *cpu, uint16_t src);
void mos6502_BVC(MOS6502 *cpu, uint16_t src);
void mos6502_BVS(MOS6502 *cpu, uint16_t src);
void mos6502_CLC(MOS6502 *cpu, uint16_t src);
void mos6502_CLD(MOS6502 *cpu, uint16_t src);
void mos6502_CLI(MOS6502 *cpu, uint16_t src);
void mos6502_CLV(MOS6502 *cpu, uint16_t src);
void mos6502_CMP(MOS6502 *cpu, uint16_t src);
void mos6502_CPX(MOS6502 *cpu, uint16_t src);
void mos6502_CPY(MOS6502 *cpu, uint16_t src);
void mos6502_DEC(MOS6502 *cpu, uint16_t src);
void mos6502_DEX(MOS6502 *cpu, uint16_t src);
void mos6502_DEY(MOS6502 *cpu, uint16_t src);
void mos6502_EOR(MOS6502 *cpu, uint16_t src);
void mos6502_INC(MOS6502 *cpu, uint16_t src);
void mos6502_INX(MOS6502 *cpu, uint16_t src);
void mos6502_INY(MOS6502 *cpu, uint16_t src);
void mos6502_JMP(MOS6502 *cpu, uint16_t src);
void mos6502_JSR(MOS6502 *cpu, uint16_t src);
void mos6502_LDA(MOS6502 *cpu, uint16_t src);
void mos6502_LDX(MOS6502 *cpu, uint16_t src);
void mos6502_LDY(MOS6502 *cpu, uint16_t src);
void mos6502_LSR(MOS6502 *cpu, uint16_t src);
void mos6502_LSR_AccMode(MOS6502 *cpu, uint16_t src);
void mos6502_NOP(MOS6502 *cpu, uint16_t src);
void mos6502_ORA(MOS6502 *cpu, uint16_t src);
void mos6502_PHA(MOS6502 *cpu, uint16_t src);
void mos6502_PHP(MOS6502 *cpu, uint16_t src);
void mos6502_PLA(MOS6502 *cpu, uint16_t src);
void mos6502_PLP(MOS6502 *cpu, uint16_t src);
void mos6502_ROL(MOS6502 *cpu, uint16_t src);
void mos6502_ROL_AccMode(MOS6502 *cpu, uint16_t src);
void mos6502_ROR(MOS6502 *cpu, uint16_t src);
void mos6502_ROR_AccMode(MOS6502 *cpu, uint16_t src);
void mos6502_RTI(MOS6502 *cpu, uint16_t src);
void mos6502_RTS(MOS6502 *cpu, uint16_t src);
void mos6502_SBC(MOS6502 *cpu, uint16_t src);
void mos6502_SEC(MOS6502 *cpu, uint16_t src);
void mos6502_SED(MOS6502 *cpu, uint16_t src);
void mos6502_SEI(MOS6502 *cpu, uint16_t src);
void mos6502_STA(MOS6502 *cpu, uint16_t src);
void mos6502_STX(MOS6502 *cpu, uint16_t src);
void mos6502_STY(MOS6502 *cpu, uint16_t src);
void mos6502_TAX(MOS6502 *cpu, uint16_t src);
void mos6502_TAY(MOS6502 *cpu, uint16_t src);
void mos6502_TSX(MOS6502 *cpu, uint16_t src);
void mos6502_TXA(MOS6502 *cpu, uint16_t src);
void mos6502_TXS(MOS6502 *cpu, uint16_t src);
void mos6502_TYA(MOS6502 *cpu, uint16_t src);
