#include "cpu6502.asm"

#bankdef zeropage { #addr 0x0000, #size 0x0100, #outp 8 * 0x0000 }
#bankdef stack    { #addr 0x0100, #size 0x0100 }
#bankdef ram      { #addr 0x0200, #size 0xFDFA, #outp 8 * 0x0200 }
#bankdef vectors  { #addr 0xFFFA, #size 0x0006, #outp 8 * 0xFFFA }

#bank ram

UNAME_BUF_LEN = 20

; rst defines program entry point
rst:

; Reads in data from the "received_data" buffer into the "user_name" buffer.
; It copies until it reads a null terminating '\0' character. It's an unsafe block.
receive_username:
    ldx #0  ; offset into received_data to read from and into user_name to write to
    .copy_username_loop:
        lda received_data, x  ; read x-th byte from received_data
        sta user_name, x      ; write that byte to x-t byte in user_name buffer

        ; if that byte is a null terminator, then we are done with the copy
        cmp #"\0"
        beq greeter

        ; otherwise, increment the pointer and continue
        inx
        jmp .copy_username_loop

; the user's name will be stored here, in a UNAME_BUF_LEN sized byte buffer
user_name:  #res UNAME_BUF_LEN

; This block writes the "Hello, " followed by the string in block user_name
; to the beginning of the Zero Page.

; It is our attack target. There is another block of code which "receives"
; the user name "from an IO device" and writes it into the user_name buffer.

; The receive block will not check the input data for a null terminator, and
; so ill-formed input that is lacking a null terminator will overflow the end
; of the user_name buffer.

; Since the attack target is located after the buffer in memory, a sufficiently
; overflowed buffer will run into this block of code. If the input data is
; maliciously crafted, arbitrary code could be injected into this block
; when execution jumps to this location, instead of being greeted on the Zero
; Page, it will be revealed that they had been pwned.
greeter:
    ; while x < .hello_len, write hello[x] to zp[x]
    .write_hello:
        ldx #0

        ..loop:
            cpx #.hello_len
            beq .write_name

            ldy .hello, x
            sty 0, x

            inx
            jmp ..loop


    .write_name:
        ldx #0

        ..loop:
            cpx #UNAME_BUF_LEN
            beq .done_writing_name

            ldy user_name, x

            cpy #"\0"
            beq .done_writing_name

            sty .hello_len, x

            inx
            jmp ..loop

    .done_writing_name:
        ldy #"!"
        sty .hello_len, x

        hlt

    .hello:
        #d "Hello, "
    .hello_len = $ - .hello

#res 1024
nmi:
irq:
    rti

; Assume this data is received byte-by-byte over some serial connection (UART/RS-232)
received_data:
    #d incbin("malicious_code.bin")

#bank vectors
#d16 nmi[7:0] @ nmi[15:8]
#d16 rst[7:0] @ rst[15:8]
#d16 irq[7:0] @ irq[15:8]