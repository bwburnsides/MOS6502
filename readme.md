# MOS 6502 Emulator

This is a emulator of the MOS 6502 8-bit microprocessor written in C. The current goal is to have full legal opcode support. After that has been validated, I may add support for the illegal opcodes.

Currently, the BCD version of the arithmetic instructions have not been implemented.

The source is currently configured to read in a 64K binary image that represents the entire address space of the processor, named `bubble.bin`, which can be built using the provided `cpu6502.asm` and `bubble.asm` source files. This code is modified from here:

http://6502.org/source/sorting/bubble8.htm

And most of the instructions have been implemented based on information from here:

https://www.masswerk.at/6502/6502_instruction_set.html

The assembler I used to build the 6502 source is customasm:

https://github.com/hlorenzi/customasm

In fact, `cpu6502.asm` is a very lightly modified version of their NES example.