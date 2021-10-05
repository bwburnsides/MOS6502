#include "cpu6502.asm"

#bankdef zeropage { #addr 0x0000, #size 0x0100, #outp 8 * 0x0000 }
#bankdef stack    { #addr 0x0100, #size 0x0100 }
#bankdef ram      { #addr 0x0200, #size 0x7E00, #outp 8 * 0x0200 }
#bankdef rom      { #addr 0x8000, #size 0x7FFA, #outp 8 * 0x8000 }
#bankdef vectors  { #addr 0xFFFA, #size 0x0006, #outp 8 * 0xFFFA }

TEMP = 0xfe
IDX = 0xff
MAX_N = 13

#bank rom
rst:
    ldy #0
    sty IDX

main:
    ldx IDX         ; Determine which FIB we want to calculate
    jsr fib         ; Calculate it, now it should be in A

    ldx IDX         ; Write the result to zeropage and see if should continue
    sta 0x00, x
    inx
    stx IDX
    cpx #MAX_N

    bne main        ; Continue until *IDX == 0
    hlt

fib:                ; calculates F(x)
    txa
    bne zeroth      ; f(0) = 0? no, continue
    rts             ; yes

zeroth:
    dex             ; prepare for f(x-1) call
    bne first       ; f(1) = 1? no, continue
    rts             ; yes

first:
    txa             ; save X on stack
    pha
    jsr fib         ; result of f(x-1) in accumulator
    tay             ; let’s put f(x-1) aside
    pla             ; get X back from stack
    tax
    tya             ; get f(x-1) back
    pha             ; save that for now on stack
    dex             ; prepare f(x-2)
    jsr fib
    sta TEMP        ; store f(x-2) for addition
    pla             ; f(x-1) from stack
    clc
    adc TEMP        ; f(x-1) + f(x-2)
    rts

irq:
nmi:
    rti

#bank vectors
#d16   nmi[7:0] @  nmi[15:8]
#d16   rst[7:0] @  rst[15:8]
#d16   irq[7:0] @  irq[15:8]