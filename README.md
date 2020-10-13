# 2-Pass-Assembler
C implementation of a 2 Pass Assembler to convert assembly code into machine level language covering major commands. Assembly level code supports the following assembly instructions:

1. MOV 2. ADD 3. SUB 4. MUL 5. CMP 6. AND 7. OR 8. NOT 9. JMP 10. LOOP 11. HLT 12. JNZ

The input to the program is an assembly language program written with instructions from the above set. The C program converts this input to a machine-level code. The conversion is done with two passes:

- In the first pass, the assembler generates the Symbol Table and Machine-Opcode Table.
- In the second pass, the assembler uses the tables from the first pass to generate the final machine level code.
