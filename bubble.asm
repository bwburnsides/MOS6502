#include "cpu6502.asm"

#bankdef zeropage { #addr 0x0000, #size 0x0100, #outp 8 * 0x0000 }
#bankdef stack    { #addr 0x0100, #size 0x0100 }
#bankdef ram      { #addr 0x0200, #size 0x7E00, #outp 8 * 0x0200 }
#bankdef rom      { #addr 0x8000, #size 0x7FFA, #outp 8 * 0x8000 }
#bankdef vectors  { #addr 0xFFFA, #size 0x0006, #outp 8 * 0xFFFA }

#bank zeropage
#addr 0x0003
#d8 8, 2, 3, 4, 9, 8, 7, 6, 5

#bank rom
rst:
    lda #0x03
    sta 0x30
    lda #0x00
    sta 0x31
    jsr sort8
    hlt

sort8:
    ldy #0x00       ; turn exchange flag off (= 0)
    sty 0x32
    lda (0x30), y   ; fetch element count
    tax             ; and put it into X
    iny             ; point to first element in list
    dex             ; decrement element count

nxtel:
    lda (0x30), y   ; fetch element
    iny
    cmp (0x30), y   ; it is larger than the next element?
    bcc chkend
    beq chkend
                    ; yes. exchange elements in memory
    pha             ; by saving low byte on stack.
    lda (0x30), y   ; then get high byte and
    dey             ; store it at low address
    sta (0x30), y
    pla             ; pull low byte from stack
    iny             ; and store it at high address
    sta (0x30), y
    lda #0xFF       ; turn exchange flag on (= -1)
    sta 0x32

chkend:
    dex             ; end of list?
    bne nxtel       ; no. fetch next element
    lda 0x32
    bne sort8
    rts             ; yes. list is now ordered


nmi:
irq:
    rti


#bank vectors
#d16   nmi[7:0] @  nmi[15:8]
#d16   rst[7:0] @  rst[15:8]
#d16   irq[7:0] @  irq[15:8]