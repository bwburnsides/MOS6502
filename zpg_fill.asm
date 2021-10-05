#include "cpu6502.asm"

#bankdef zeropage { #addr 0x0000, #size 0x0100, #outp 8 * 0x0000 }
#bankdef stack    { #addr 0x0100, #size 0x0100 }
#bankdef ram      { #addr 0x0200, #size 0x7E00, #outp 8 * 0x0200 }
#bankdef rom      { #addr 0x8000, #size 0x7FFA, #outp 8 * 0x8000 }
#bankdef vectors  { #addr 0xFFFA, #size 0x0006, #outp 8 * 0xFFFA }

#bank rom
rst:
    ldx #0x00
    ldy #0xff

loop:

    sty 0x00, x

    inx
    dey

    bne loop
    hlt

irq:
nmi:
    rti

#bank vectors
#d16   nmi[7:0] @  nmi[15:8]
#d16   rst[7:0] @  rst[15:8]
#d16   irq[7:0] @  irq[15:8]