#include "cpu6502.asm"

#bankdef zeropage { #addr 0x0000, #size 0x0100, #outp 8 * 0x0000 }
#bankdef stack    { #addr 0x0100, #size 0x0100 }
#bankdef ram      { #addr 0x0200, #size 0x7E00, #outp 8 * 0x0200 }
#bankdef rom      { #addr 0x8000, #size 0x7FFA, #outp 8 * 0x8000 }
#bankdef vectors  { #addr 0xFFFA, #size 0x0006, #outp 8 * 0xFFFA }

#bank rom
main:
    jsr foo
    hlt

nmi:
irq:
    rti

foo:
    nop
    nop
    rts

#bank vectors
#d16   nmi[7:0]  @  nmi[15:8]
#d16   main[7:0] @  main[15:8]
#d16   irq[7:0]  @  irq[15:8]