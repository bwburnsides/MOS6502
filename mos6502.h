#include <inttypes.h>

#define C_Mask ((uint8_t) 0x01)     // 0b00000001
#define Z_Mask ((uint8_t) 0x02)     // 0b00000010
#define I_Mask ((uint8_t) 0x04)     // 0b00000100
#define D_Mask ((uint8_t) 0x08)     // 0b00001000
#define B_Mask ((uint8_t) 0x10)     // 0b00010000
#define BIT5_Mask ((uint8_t) 0x20)  // 0b00100000
#define V_Mask ((uint8_t) 0x40)     // 0b01000000
#define N_Mask ((uint8_t) 0x80)     // 0b10000000

#define SET_C(x) ((x) ? (cpu->P |= C_Mask) : (cpu->P &= (~C_Mask)))
#define SET_Z(x) ((x) ? (cpu->P |= Z_Mask) : (cpu->P &= (~Z_Mask)))
#define SET_I(x) ((x) ? (cpu->P |= I_Mask) : (cpu->P &= (~I_Mask)))
#define SET_D(x) ((x) ? (cpu->P |= D_Mask) : (cpu->P &= (~D_Mask)))
#define SET_B(x) ((x) ? (cpu->P |= B_Mask) : (cpu->P &= (~B_Mask)))
#define SET_5(x) ((x) ? (cpu->P |= BIT5_Mask) : (cpu->P &= (~BIT5_Mask)))
#define SET_V(x) ((x) ? (cpu->P |= V_Mask) : (cpu->P &= (~V_Mask)))
#define SET_N(x) ((x) ? (cpu->P |= N_Mask) : (cpu->P &= (~N_Mask)))

#define IS_C ((cpu->P & C_Mask) ? 1 : 0)
#define IS_Z ((cpu->P & Z_Mask) ? 1 : 0)
#define IS_I ((cpu->P & I_Mask) ? 1 : 0)
#define IS_D ((cpu->P & D_Mask) ? 1 : 0)
#define IS_B ((cpu->P & B_Mask) ? 1 : 0)
#define IS_5 ((cpu->P & BIT5_Mask) ? 1 : 0)
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
    uint8_t IR;

    uint8_t nmi;
    uint8_t irq;

    Instruction instructions[256];
};

MOS6502 *mos6502(MemRead read, MemWrite write);
void mos6502_rst(MOS6502 *cpu);
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

#define BRK_IMP 0x00
#define BPL_REL 0x10
#define JSR_ABS 0x20
#define BMI_REL 0x30
#define RTI_IMP 0x40
#define BVC_REL 0x50
#define RTS_IMP 0x60
#define BVS_REL 0x70
// #define 0x80
#define BCC_REL 0x90
#define LDY_IMM 0xA0
#define BCS_REL 0xB0
#define CPY_IMM 0xC0
#define BNE_REL 0xD0
#define CPX_IMM 0xE0
#define BEQ_REL 0xF0

#define ORA_XIDX_IND 0x01
#define ORA_IND_YIDX 0x11
#define AND_XIDX_IND 0x21
#define AND_IND_YIDX 0x31
#define EOR_XIDX_IND 0x41
#define EOR_IND_YIDX 0x51
#define ADC_XIDX_IND 0x61
#define ADC_IND_YIDX 0x71
#define STA_XIDX_IND 0x81
#define STA_IND_YIDX 0x91
#define LDA_XIDX_IND 0xA1
#define LDA_IND_YIDX 0xB1
#define CMP_XIDX_IND 0xC1
#define CMP_IND_YIDX 0xD1
#define SBC_XIDX_IND 0xE1
#define SBC_IND_YIDX 0xF1

// #define 0x02
// #define 0x12
// #define 0x22
// #define 0x32
// #define 0x42
// #define 0x52
// #define 0x62
// #define 0x72
// #define 0x82
// #define 0x92
#define LDX_IMM 0xA2
// #define 0xB2
// #define 0xC2
// #define 0xD2
// #define 0xE2
// #define 0xF2

// #define 0x03
// #define 0x13
// #define 0x23
// #define 0x33
// #define 0x43
// #define 0x53
// #define 0x63
// #define 0x73
// #define 0x83
// #define 0x93
// #define 0xA3
// #define 0xB3
// #define 0xC3
// #define 0xD3
// #define 0xE3
// #define 0xF3

// #define 0x04
// #define 0x14
#define BIT_ZPG      0x24
// #define 0x34
// #define 0x44
// #define 0x54
// #define 0x64
// #define 0x74
#define STY_ZPG      0x84
#define STY_ZPG_XIDX 0x94
#define LDY_ZPG      0xA4
#define LDY_ZPG_XIDX 0xB4
#define CPY_ZPG      0xC4
// #define 0xD4
#define CPX_ZPG      0xE4
// #define 0xF4

#define ORA_ZPG      0x05
#define ORA_ZPG_XIDX 0x15
#define AND_ZPG      0x25
#define AND_ZPG_XIDX 0x35
#define EOR_ZPG      0x45
#define EOR_ZPG_XIDX 0x55
#define ADC_ZPG      0x65
#define ADC_ZPG_XIDX 0x75
#define STA_ZPG      0x85
#define STA_ZPG_XIDX 0x95
#define LDA_ZPG      0xA5
#define LDA_ZPG_XIDX 0xB5
#define CMP_ZPG      0xC5
#define CMP_ZPG_XIDX 0xD5
#define SBC_ZPG      0xE5
#define SBC_ZPG_XIDX 0xF5

#define ASL_ZPG      0x06
#define ASL_ZPG_XIDX 0x16
#define ROL_ZPG      0x26
#define ROL_ZPG_XIDX 0x36
#define LSR_ZPG      0x46
#define LSR_ZPG_XIDX 0x56
#define ROR_ZPG      0x66
#define ROR_ZPG_XIDX 0x76
#define STX_ZPG      0x86
#define STX_ZPG_YIDX 0x96
#define LDX_ZPG      0xA6
#define LDX_ZPG_YIDX 0xB6
#define DEC_ZPG      0xC6
#define DEC_ZPG_XIDX 0xD6
#define INC_ZPG      0xE6
#define INC_ZPG_XIDX 0xF6

// #define 0x07
// #define 0x17
// #define 0x27
// #define 0x37
// #define 0x47
// #define 0x57
// #define 0x67
// #define 0x77
// #define 0x87
// #define 0x97
// #define 0xA7
// #define 0xB7
// #define 0xC7
// #define 0xD7
// #define 0xE7
// #define 0xF7

#define PHP_IMP 0x08
#define CLC_IMP 0x18
#define PLP_IMP 0x28
#define SEC_IMP 0x38
#define PHA_IMP 0x48
#define CLI_IMP 0x58
#define PLA_IMP 0x68
#define SEI_IMP 0x78
#define DEY_IMP 0x88
#define TYA_IMP 0x98
#define TAY_IMP 0xA8
#define CLV_IMP 0xB8
#define INY_IMP 0xC8
#define CLD_IMP 0xD8
#define INX_IMP 0xE8
#define SED_IMP 0xF8

#define ORA_IMM      0x09
#define ORA_ABS_YIDX 0x19
#define AND_IMM      0x29
#define AND_ABS_YIDX 0x39
#define EOR_IMM      0x49
#define EOR_ABS_YIDX 0x59
#define ADC_IMM      0x69
#define ADC_ABS_YIDX 0x79
// #define 0x89
#define STA_ABS_YIDX 0x99
#define LDA_IMM      0xA9
#define LDA_ABS_YIDX 0xB9
#define CMP_IMM      0xC9
#define CMP_ABS_YIDX 0xD9
#define SBC_IMM      0xE9
#define SBC_ABS_YIDX 0xF9

#define ASL_ACC 0x0A
// #define 0x1A
#define ROL_ACC 0x2A
// #define 0x3A
#define LSR_ACC 0x4A
// #define 0x5A
#define ROR_ACC 0x6A
// #define 0x7A
#define TXA_IMP 0x8A
#define TXS_IMP 0x9A
#define TAX_IMP 0xAA
#define TSX_IMP 0xBA
#define DEX_IMP 0xCA
// #define 0xDA
#define NOP_IMP 0xEA
// #define 0xFA

// #define 0x0B
// #define 0x1B
// #define 0x2B
// #define 0x3B
// #define 0x4B
// #define 0x5B
// #define 0x6B
// #define 0x7B
// #define 0x8B
// #define 0x9B
// #define 0xAB
// #define 0xBB
// #define 0xCB
// #define 0xDB
// #define 0xEB
// #define 0xFB

// #define 0x0C
// #define 0x1C
#define BIT_ABS      0x2C
// #define 0x3C
#define JMP_ABS      0x4C
// #define 0x5C
#define JMP_IND      0x6C
// #define 0x7C
#define STY_ABS      0x8C
// #define 0x9C
#define LDY_ABS      0xAC
#define LDY_ABS_XIDX 0xBC
#define CPY_ABS      0xCC
// #define 0xDC
#define CPX_ABS      0xEC
// #define 0xFC

#define ORA_ABS      0x0D
#define ORA_ABS_XIDX 0x1D
#define AND_ABS      0x2D
#define AND_ABS_XIDX 0x3D
#define EOR_ABS      0x4D
#define EOR_ABS_XIDX 0x5D
#define ADC_ABS      0x6D
#define ADC_ABS_XIDX 0x7D
#define STA_ABS      0x8D
#define STA_ABS_XIDX 0x9D
#define LDA_ABS      0xAD
#define LDA_ABS_XIDX 0xBD
#define CMP_ABS      0xCD
#define CMP_ABS_XIDX 0xDD
#define SBC_ABS      0xED
#define SBC_ABS_XIDX 0xFD

#define ASL_ABS      0x0E
#define ASL_ABS_XIDX 0x1E
#define ROL_ABS      0x2E
#define ROL_ABS_XIDX 0x3E
#define LSR_ABS      0x4E
#define LSR_ABS_XIDX 0x5E
#define ROR_ABS      0x6E
#define ROR_ABS_XIDX 0x7E
#define STX_ABS      0x8E
// #define 0x9E
#define LDX_ABS      0xAE
#define LDX_ABS_YIDX 0xBE
#define DEC_ABS      0xCE
#define DEC_ABS_XIDX 0xDE
#define INC_ABS      0xEE
#define INC_ABS_XIDX 0xFE

// #define 0x0F
// #define 0x1F
// #define 0x2F
// #define 0x3F
// #define 0x4F
// #define 0x5F
// #define 0x6F
// #define 0x7F
// #define 0x8F
// #define 0x9F
// #define 0xAF
// #define 0xBF
// #define 0xCF
// #define 0xDF
// #define 0xEF
#define HLT_IMP 0xFF  // NOTE: this is a custom opcode