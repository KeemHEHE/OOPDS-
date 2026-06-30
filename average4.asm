; average4.asm - reads 4 values and displays their average
INPUT R0
INPUT R1
INPUT R2
INPUT R3
ADD R0, R1
ADD R0, R2
ADD R0, R3
MOV R4, 4
DIV R0, R4
DISPLAY R0
