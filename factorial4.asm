; factorial4.asm - calculates 4! (4 factorial = 1*2*3*4 = 24) and displays the result
; no loop/branch instructions exist in this assembly language, so the multiplication
; is unrolled manually for the fixed value 4
MOV R0, 1
MOV R1, 2
MUL R0, R1
MOV R1, 3
MUL R0, R1
MOV R1, 4
MUL R0, R1
DISPLAY R0
