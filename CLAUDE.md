# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Context

This is a C++ academic assignment for **CCP6124 OOPDS (Object-Oriented Programming & Data Structures)**, Term 2610. The assignment implements a **Virtual Machine and Assembly Language Interpreter** in C++. See `CCP6124 OOPDS Term 2610 Assignment - Virtual Machine.pdf` for the full specification.

**Due:** 4 July 2026, 12:00AM | **Submission:** Single `.cpp` file + PDF report + MPEG video

## VM Architecture

| Component | Spec |
|-----------|------|
| Registers | R0–R7, `signed char` (-128 to 127) |
| Program Counter (PC) | 1 byte, starts at 0, increments after each instruction |
| Stack Index (SI) | 1 byte, incremented on PUSH, decremented on POP |
| Flags | OF (overflow >127), UF (underflow <-128), CF (carry), ZF (zero) |
| Memory | 64 `signed char` bytes, addresses 0–63 |

## Required Class Structure

```
Register (base class)
└── GeneralRegister (R0–R7)

FlagRegister (aggregated by CPU)
Memory (composition inside CPU)

Instruction (abstract base, virtual execute())
├── ArithmeticInstruction
├── IOInstruction
└── ShiftInstruction

CPU (owns Memory, uses FlagRegister, holds registers + PC)
Runner (loads .asm file, stores vector<Instruction*>, executes via CPU)
```

OOP requirements:
- **Encapsulation:** all members private, access via getters/setters
- **Inheritance:** at least 2 places
- **Polymorphism:** `vector<Instruction*>` with virtual `execute()`, at least 2 places
- **Composition:** CPU owns Memory | **Aggregation:** CPU uses FlagRegister

## Data Structures (must implement own — no STL containers)

- `List` / `Vector` — instruction storage, iterating the program
- `Stack` — PUSH/POP operations and execution tracking
- `Queue` — optional program storage

## Assembly Instructions to Implement

| Category | Instructions |
|----------|-------------|
| I/O | `INPUT <Reg>`, `DISPLAY <Reg>` |
| Move | `MOV Rd, imm` / `MOV Rd, Rs` / `MOV Rd, [Rs]` |
| Arithmetic | `ADD`, `SUB`, `MUL`, `DIV`, `INC`, `DEC` |
| Rotate | `ROL Rd, count`, `ROR Rd, count` |
| Shift | `SHL Rd, count`, `SHR Rd, count` |
| Memory | `LOAD Rd, [addr]` / `LOAD Rd, [Rs]`, `STORE Rd, addr` / `STORE [Rd], Rs` |
| Flags | `RESET <CF\|OF\|UF\|ZF>` |
| Stack | `PUSH <Reg>`, `POP <Reg>` |

Flags (OF, UF, CF, ZF) must be updated after every operation that changes a register.

## Output Format (must match exactly — letter casing, spacing, `#` only)

```
#Begin#
#Registers#0000#0011#0000#0044#0000#0000#0000#0000#
#Flags#0#0#0#0#
#PC#0006#
#Memory#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0044#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#End#
```

Values are zero-padded to 4 digits. Memory is 8 rows × 8 columns.

## Current Implementation Status (`practice.cpp`)

- [x] `Register` — `signed char value`, setValue/getValue with clamping, display
- [x] `FlagRegister` — OF/UF/CF/ZF with getters/setters/reset/display
- [x] `Memory` — 64-byte array skeleton (read/write stubs not yet implemented)
- [ ] `GeneralRegister` — derive from `Register`
- [ ] `Instruction` hierarchy — abstract base + derived classes
- [ ] `CPU` — compose Memory, aggregate FlagRegister, hold registers + PC + SI
- [ ] `Runner` — load `.asm` file, decode, execute via CPU
- [ ] Custom data structures (List, Stack, Queue)

## Build & Run

```bash
g++ -o output_name file.cpp && ./output_name
```

For C++17 features:
```bash
g++ -std=c++17 -o output_name file.cpp && ./output_name
```

There is no build system (no Makefile); compile individual files directly.

## Code Style

- Use `class` with `private`/`public` access modifiers for encapsulation
- `practice.cpp` is the main working file
- No external libraries — standard library (`<iostream>`, `<string>`, `<vector>`, etc.) only
- Functions must not exceed 35 lines (assignment rule — penalty per violation)
- Each class/function must have a comment identifying which group member wrote it
