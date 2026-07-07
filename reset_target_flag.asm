; reset_target_flag.asm - bare RESET syntax for each target flag
; note: flags start at 0, so running this alone shows no visible change --
; see reset_all_demo.asm for a version that sets each flag before resetting it

RESET OF     ; targets Overflow Flag only
RESET UF     ; targets Underflow Flag only
RESET CF     ; targets Carry Flag only
RESET ZF     ; targets Zero Flag only
