; reset_all_demo.asm - sets each flag (OF, UF, CF, ZF) then resets it, one at a time

; --- OF (Overflow: result > 127) ---
MOV R0, 100
ADD R0, 50    ; 150 > 127 -> OF = 1, CF = 1
RESET OF      ; OF back to 0

; --- UF (Underflow: result < -128) ---
MOV R1, -100
SUB R1, 50    ; -150 < -128 -> UF = 1, CF = 1
RESET UF      ; UF back to 0

; --- CF (Carry: set alongside OF or UF on overflow/underflow) ---
MOV R2, 100
ADD R2, 50    ; overflow -> CF = 1 (OF also set again here)
RESET CF      ; CF back to 0

; --- ZF (Zero: result == 0) ---
MOV R6, 5
SUB R6, R6    ; 5 - 5 = 0 -> ZF = 1
RESET ZF      ; ZF back to 0
