Team members:
    Prashant Sharma: 14ucs085
    Shashwat Shalvi :14ucs109
    Shubham Gupta: 14ucs116
    Vidhan Jain: 14ucs141



Program 1:
    Check whether a number is palindrome or not.

Program 2:
    Selection Sort for 3 numbers.


code1.txt: Assembly language code for Program1
code2.txt: Assembly language code for Program2
opcode.txt: List of all opcodes used in assembly code and their binary representation
reg.txt: List of all registers
pass1.c: C program to generate label table from the assembly code
pass2.c: C program to generate binary machine code using assembly code and label table genearted using pass1.c
binary_to_output.c: C program to execute the binary machine code
input1.txt: Number to be checked for palindrome (for code1)
input2.txt: 3 numbers to be sorted (for code2)
label1.txt, label2.txt: Label file generated using pass1.c
binary1.txt, binary2.txt: Binary machine code generated using pass2.c


STEP1: Run pass1.c
    This will generate the label table for the given assembly code.

STEP2: Run pass2.c
    This will generate binary machine code for the given assembly code.

STEP3: Run binary_to_output.c
    This will execute the binary machine code generated in STEP2.


