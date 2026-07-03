# Video Script — Adam's Segment (Core Architecture)

Spec requirement (§7.3): each member explains their own contribution, shows part of
the code they wrote, and demos compiling/running the program. Read this as a guide,
not word-for-word — say it in your own voice.

Estimated time: ~4-5 minutes.

---

## 1. Intro (15-20s)

> "Hi, I'm Adam, student ID 252UC242PM. My contribution to this assignment is the
> core architecture of the virtual machine: the `Register`, `GeneralRegister`,
> `FlagRegister`, `Memory`, `Stack`, and `CPU` classes, plus the `RESET`, `PUSH`,
> and `POP` instructions, and integrating everyone's work together into the final
> `TT9L_I.cpp` file."

---

## 2. Code walkthrough (2-2.5 min)

Open `TT9L_I.cpp` in the editor and scroll to each class as you talk. Line numbers
below are just for you to navigate quickly while recording.

### Register — line 14
> "This is the `Register` base class. The value is a private `signed char`, so it's
> naturally clamped to -128 to 127 like the spec requires. It's only accessed
> through `getValue`/`setValue` — that's encapsulation. It also has a virtual
> destructor, which sets it up to be a base class for inheritance."

### GeneralRegister — line 51
> "`GeneralRegister` inherits from `Register` and adds an `id`, 0 through 7, for
> R0 to R7. This is one of the two required inheritance relationships."

### FlagRegister — line 68
> "`FlagRegister` holds OF, UF, CF, and ZF, all private, with getters and setters
> for each. The CPU doesn't own this — it's passed in by reference — which is the
> aggregation relationship the spec asks for."

### Memory — line 120
> "`Memory` wraps the 64-byte `signed char` array the spec requires. `read` and
> `write` are bounds-checked against 0-63, and the array itself is private."

### Stack — line 153
> "`Stack` is our own LIFO implementation — a fixed 64-byte array plus a top index,
> no `std::stack`. `push`, `pop`, `peek`, `isEmpty`, `isFull` are the only way to
> touch it. This is what backs `PUSH`/`POP` and drives the stack index."

### CPU — line 216
> "`CPU` ties everything together. It **composes** a `Memory` object — the CPU
> creates it and it dies with the CPU. It **aggregates** a `FlagRegister` — passed
> in by reference, so it exists independently. It holds the 8 `GeneralRegister`s,
> the `Stack`, the program counter, and the stack index, all private, exposed only
> through getters and controlled methods like `incrementPC`, `pushStack`,
> `popStack`, and `updateFlags`, which sets OF/UF/CF/ZF after arithmetic."

### RESET / PUSH / POP — lines 765, 785, 801
> "I also implemented the `RESET`, `PUSH`, and `POP` instructions. Each one
> inherits from the abstract `Instruction` base class and overrides `execute()` —
> that's the polymorphism: they all get called through an `Instruction*` in the
> program's instruction list. `PUSH` and `POP` are the ones that actually drive
> the `Stack` class underneath."

### Integration (10s)
> "Beyond my own classes, I also integrated Adeeb's and Ammar's instruction
> classes with my CPU and memory model so everything compiles and runs together
> as one program, in one file, as the spec requires."

---

## 3. Live demo (1.5-2 min)

Open a terminal in the project folder. Type these live — don't paste — so it's
clearly a real compile, not a pre-recorded clip.

### Compile
```bash
g++ -std=c++17 -o TT9L_I TT9L_I.cpp
```
> "Compiling from the command line with g++, C++17, no errors."

### Run 1 — factorial4.asm (no input needed)
```bash
./TT9L_I factorial4.asm
```
> "This computes 4 factorial with a MOV and three MULs. R0 ends at 24, and you
> can see the final register, flag, PC, and memory dump in the exact `#Begin#...
> #End#` format the spec requires."

### Run 2 — overflow_demo.asm (shows a flag actually flip)
```bash
./TT9L_I overflow_demo.asm
```
> "This one adds 100 to itself, which is 200 — past the signed char range of 127.
> You can see OF is now 1 in the flags line, and the value in R0 has been clamped."

### Run 3 — sum5.asm (shows INPUT working live)
```bash
./TT9L_I sum5.asm
```
> "This prompts for 5 values with INPUT and sums them into R0."

Type `1`, `2`, `3`, `4`, `5`, pressing Enter after each.

> "R0 comes out to 15 — 1 plus 2 plus 3 plus 4 plus 5 — confirming INPUT, ADD, and
> DISPLAY all work correctly together."

(Optional, if time allows) `./TT9L_I factorial4.asm --step` to show the full VM
state dumped after every single instruction, not just at the end.

---

## 4. Closing (5-10s)

> "That covers my part — the core architecture, the stack, and the CPU that ties
> it all together."

---

## Cleanup after recording

The compiled `TT9L_I` binary and `output.txt` are build artifacts — delete them
before zipping the submission folder if they get created in the project directory:
```bash
rm -f TT9L_I output.txt
```
