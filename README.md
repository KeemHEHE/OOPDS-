# Virtual Machine and Assembly Language Interpreter

CCP6124 OOPDS (Term 2610) group assignment — a simplified 8-bit virtual machine and
its assembly language interpreter, written in C++ with custom data structures (no STL).

See `Virtualmachine2610v1.21 (1).pdf` for the full assignment spec, and
`REPORT_DRAFT.md` for the architecture report (work in progress).

## Build

```bash
g++ -std=c++17 -o TT9L_I TT9L_I.cpp
```

## Run

```bash
./TT9L_I                     # runs test_program.asm by default
./TT9L_I myfile.asm          # runs a specific .asm file
./TT9L_I myfile.asm --step   # dumps the full VM state after every instruction
                              # (instead of just at the end) -- useful for debugging
```

`INPUT` instructions prompt with `?` — type a number and press Enter.

The final VM state is printed to the screen **and** written to `output.txt`.

## Example programs included

| File | What it does |
|------|--------------|
| `sum5.asm` | Sums 5 user-entered values |
| `average4.asm` | Averages 4 user-entered values |
| `factorial4.asm` | Computes 4! (no input needed) |
| `overflow_demo.asm` | Demonstrates the OF flag being set |
| `spec_sample.asm` | The assignment spec's own worked example |
| `test_program.asm` | Exercises every supported instruction |

## Instruction set

```
MOV Rd, imm        e.g. MOV R0, 10        (R0 = 10)
MOV Rd, Rs          e.g. MOV R0, R1        (R0 = R1)
MOV Rd, [Rs]        e.g. MOV R0, [R1]      (R0 = memory at address in R1)
ADD/SUB/MUL/DIV Rd, Rs-or-imm   e.g. ADD R0, R1   or   ADD R0, 5
INC Rd / DEC Rd     e.g. INC R0
ROL/ROR Rd, count   e.g. ROL R0, 2
SHL/SHR Rd, count   e.g. SHL R0, 1
LOAD Rd, [addr]     e.g. LOAD R0, [20]
LOAD Rd, [Rs]       e.g. LOAD R0, [R1]
STORE Rs, addr      e.g. STORE R0, 20
STORE Rs, [Rd]      e.g. STORE R0, [R1]
RESET OF/UF/CF/ZF   e.g. RESET ZF
PUSH Rd / POP Rd
INPUT Rd / DISPLAY Rd
```

`;` starts a comment; blank lines are ignored; more than one instruction on a line
is a fatal parse error.

## Architecture

- `Register` → `GeneralRegister` (inheritance)
- `FlagRegister` (aggregated by `CPU`), `Memory` (composition inside `CPU`)
- `Instruction` (abstract, virtual `execute()`) → `ArithmeticInstruction` /
  `IOInstruction` / `ShiftInstruction` → 17 concrete instruction classes
- `CPU` ties registers, memory, flags, and stack together
- `Runner` reads a `.asm` file into a custom `MyVector<string>`, parses each line into
  an `Instruction*`, queues it in a custom `MyQueue`, and executes via `CPU`
- Custom `Stack`, `MyVector<T>`, `MyQueue` — no STL containers

See `TEAM_WORK_DIVISION.md` for who built what.
