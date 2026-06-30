# Team Work Division — Actual State (as of integration into `feature/adam-core`)

This is the honest, detailed version for your own reference (interview prep, and as
the source of truth behind the report's contribution table). It separates **what each
person originally built** (from their own pushed branch, before integration) from
**what was added or fixed during integration** — which is what the report's §7 AI
Tool Usage Disclosure refers to.

---

## Member 1 — Adam — Core Architecture

**Originally implemented (by Adam):**
- `Register` — signed char value with clamping (-128..127), virtual destructor
- `FlagRegister` — OF/UF/CF/ZF with getters/setters/reset
- `GeneralRegister` — inherits `Register`, adds register id (R0-R7)
- `Memory` — 64-byte bounds-checked array, read/write
- `Stack` — 64-byte array-backed, push/pop/peek/top
- `CPU` — ties registers, memory, flags, stack, PC, SI together; `updateFlags()`,
  `displayState()`

**Added during integration (Adam + AI assistance):**
- `ResetInstruction`, `PushInstruction`, `PopInstruction` — not assigned to anyone
  in the original division; gap discovered while checking the implementation
  against the spec's required instruction list
- `Stack::pop()` now exits the program on an empty stack, per spec
- `CPU`'s `FlagRegister` changed from a value member to a reference (`FlagRegister&`)
  to make aggregation vs. composition structurally real, not just descriptive
- `displayState()`: fixed a hex-vs-decimal output bug (confirmed against the spec's
  own worked example), added writing to `output.txt` in addition to the screen, then
  split into `buildDump()` + `displayState()` to stay under the 35-line limit

---

## Member 2 — Adeeb — Instructions (Part 1)

**Originally implemented (by Adeeb, from his pushed branch):**
- `Instruction` abstract base class — pure virtual `execute()`, virtual destructor
- `MyVector<T>` — custom dynamic array
- `MovInstruction` — all 3 addressing modes (immediate / register / register-indirect)
- `AddInstruction`, `SubInstruction`, `MulInstruction`, `DivInstruction` — register-only
- `IncInstruction`, `DecInstruction`

**Added/fixed during integration:**
- `ArithmeticInstruction` intermediate class — required by the spec's class diagram,
  was missing (Add/Sub/Mul/Div/Inc/Dec inherited directly from `Instruction`)
- Immediate-operand support for ADD/SUB/MUL/DIV — originally register-only, but the
  spec's own worked example uses `ADD R1, 6` (an immediate), so this was added
- Per-class `// Member 2: Adeeb` attribution comments (originally one block comment
  covering the whole section)

---

## Member 3 — Ammar — Instructions (Part 2) + Runner

**Originally implemented (by Ammar, from his pushed branch):**
- `MyQueue` — custom fixed-capacity circular queue
- `LoadInstruction`, `StoreInstruction` — direct-address mode only
- `RolInstruction`, `RorInstruction`, `ShlInstruction`, `ShrInstruction` — hardcoded
  to rotate/shift by exactly 1 bit
- `InputInstruction`, `DisplayInstruction`
- `Runner` class skeleton — `loadProgram()` hardcoded 5 demo instructions directly
  in C++; no actual `.asm` file reading or parsing existed

**Added/fixed during integration:**
- `IOInstruction`, `ShiftInstruction` intermediate classes — required by the spec's
  class diagram, were missing
- Fixed a data-corruption bug in ROL/ROR/SHL/SHR (shifting the *signed* register
  value instead of the unsigned byte pattern silently produced wrong results,
  confirmed against the spec's own bit-diagram examples)
- Added the `count` operand to ROL/ROR/SHL/SHR (e.g. `ROL R0, 3`) — originally
  hardcoded to 1
- Added register-indirect addressing (`[Rs]`/`[Rd]`) to LOAD/STORE — originally
  direct-address only
- Wrote the entire `.asm` file parser from scratch — `parseLine()`, `buildMov()`,
  `buildArithmetic()`, `buildMemory()`, `buildShift()`, `buildIO()`, `buildReset()`,
  `buildStack()`, plus tokenizing helpers (`trim()`, `parseRegister()`,
  `isBracketed()`, `stripBrackets()`)
- Rewrote `Runner::loadProgram()` to actually read and parse a file (via
  `MyVector<string>` then `MyQueue<Instruction*>`, matching the spec's described
  two-stage process)
- Added `--step` mode (dumps state after every instruction, for report screenshots)
  and command-line argument support for the `.asm` filename
- Fixed `INPUT`'s prompt to display `?` per spec (was `"Enter value: "`)

---

## Cross-cutting integration work (not owned by any single original branch)

- Found and fixed the critical hex-vs-decimal `displayState()` bug by cross-checking
  our output against the spec's own worked example numbers
- Built and ran the spec's literal sample program (`spec_sample.asm`) to verify
  output byte-for-byte; this surfaced the immediate-operand and STORE-operand-order
  issues listed above
- Added multi-instruction-per-line detection (parse error + exit, per spec)
- Made output write to both the screen and `output.txt`, per spec section 6
- Wrote the 3 required example programs (`sum5.asm`, `average4.asm`,
  `factorial4.asm`) plus `overflow_demo.asm` and `spec_sample.asm`
- Integrated all three branches' work into the single `practice.cpp`, resolving the
  unrelated-git-history problem between branches along the way
- `.gitignore` / repo cleanup (stopped tracking compiled binaries, removed an
  unrelated scratch file)

---

## How to use this

- The report's contribution table (REPORT_DRAFT.md) can stay categorical — "whose
  assigned area" — since that's the normal convention for these tables.
- This file is the detailed version for your own prep: if the interview asks "why
  does X work this way," the answer is in whichever section above covers it.
- If your coordinator's AI policy requires more granular disclosure than the current
  §7 in the report, this file has the level of detail to draw from.
