#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
using namespace std;

// Member 1: Adam
// OOP: Encapsulation (private value, accessed only via getValue/setValue) +
// Inheritance/Polymorphism (base class for GeneralRegister; virtual destructor
// allows safe deletion through a base Register*). No custom data structure here.
class Register {
    private:
        signed char value;

    public:
        // Initializes the register to 0.
        Register() {
            value = 0;
        }
        // Virtual destructor so derived registers destruct safely through a base pointer.
        virtual ~Register() {}

        // Sets the register's value, clamping it to the signed char range (-128 to 127).
        void setValue(int n) {
            if (n > 127) {
                value = 127;
            } else if (n < -128) {
                value = -128;
            } else {
                value = n;
            }
        }

        // Returns the register's current value.
        int getValue() {
            return value;
        }

        // Prints the register's current value to the console.
        void display() {
            cout << "Register value: " << (int)value << endl;
        }
};

// Member 1: Adam
// OOP: Inheritance (derives from Register) + Encapsulation (private id).
// No custom data structure here.
class GeneralRegister : public Register {
    private:
        int id;
    public:
        // Default constructor: creates a general register with id 0.
        GeneralRegister() : id(0) {}
        // Creates a general register identified by id (e.g. 0 for R0).
        GeneralRegister(int id) : id(id) {}
        // Returns this register's id (0-7, corresponding to R0-R7).
        int getID() {
            return id;
        }
};

// Member 1: Adam
// OOP: Encapsulation (private OF/UF/CF/ZF, accessed only via getters/setters).
// This is the class CPU aggregates. No custom data structure here.
class FlagRegister {
    private:
        int OF;
        int UF;
        int CF;
        int ZF;

    public:
        // Initializes all flags (OF, UF, CF, ZF) to 0.
        FlagRegister() {
            OF = 0;
            UF = 0;
            CF = 0;
            ZF = 0;
        }

        // Sets the overflow flag.
        void setOF(int v) { OF = v; }
        // Sets the underflow flag.
        void setUF(int v) { UF = v; }
        // Sets the carry flag.
        void setCF(int v) { CF = v; }
        // Sets the zero flag.
        void setZF(int v) { ZF = v; }

        // Returns the overflow flag.
        int getOF() { return OF; }
        // Returns the underflow flag.
        int getUF() { return UF; }
        // Returns the carry flag.
        int getCF() { return CF; }
        // Returns the zero flag.
        int getZF() { return ZF; }

        // Prints all four flags to the console.
        void displayFlags() {
            cout << "OF=" << OF << " UF=" << UF << " CF=" << CF << " ZF=" << ZF << endl;
        }

        // Resets all flags back to 0.
        void reset() {
            OF = 0;
            UF = 0;
            CF = 0;
            ZF = 0;
        }
};

// Member 1: Adam
// Both: OOP - Encapsulation (private data[64], accessed only via bounds-checked
// read/write). Data Structure - a fixed-size array is the underlying storage
// that models the VM's 64-byte memory.
class Memory {
    private:
        signed char data[64];

    public:
        // Initializes all 64 memory bytes to 0.
        Memory() {
            for (int i = 0; i < 64; i++) {
                data[i] = 0;
            }
        }

        // Writes value to the given address if it is within bounds (0-63).
        void write(int address, int value) {
            if (address >= 0 && address <= 63) {
                data[address] = value;
            }
        }

        // Reads the value at the given address, or 0 if out of bounds.
        int read(int address) {
            if (address >= 0 && address <= 63) {
                return data[address];
            }
            return 0;
        }
};

// Member 1: Adam
// Both: Data Structure - custom LIFO Stack (array + top index), one of the
// assignment's required self-implemented data structures (no STL). OOP -
// Encapsulation (private data[]/top, exposed only through push/pop/peek/
// isEmpty/isFull/getTop).
class Stack {
    private:
        signed char data [64];
        int top;

    public:
        // Initializes an empty stack (top = -1).
        Stack() : top(-1) {}

        // Returns true if the stack has no elements.
        bool isEmpty() {
            return top == -1;

        }

        // Returns true if the stack has reached its 64-byte capacity.
        bool isFull() {
            return top == 63;

        }

        // Pushes value onto the stack if it is not full.
        void push(int value) {
            if (!isFull()) {
                top++;
                data[top] = (signed char)value;

            }

        }

        // Pops the top value off the stack. Per spec, popping an empty stack
        // is a fatal error that stops the program.
        int pop() {
            if (isEmpty()) {
                cerr << "FATAL: POP on empty stack" << endl;
                exit(1);
            }
            int val = data[top];
            top--;
            return val;
        }

        // Returns the top value without removing it, or 0 if empty.
        int peek() {
            if (!isEmpty()) return data[top];
            return 0;

        }

        // Returns the current top index.
        int getTop() {
            return top;
        }

};

// Member 1: Adam
// Both: OOP - Composition (owns Memory, destroyed with the CPU), Aggregation
// (uses an externally-owned FlagRegister&), Encapsulation (private registers/
// memory/flags/stack/PC/SI, exposed only via getters and controlled mutators).
// Data Structure - registers[8] is a fixed array of GeneralRegister, and
// pushStack/popStack delegate to the Stack data structure above.
class CPU {
    private:
        GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
        Memory memory;       // composition: CPU owns and destroys this Memory
        FlagRegister &flags; // aggregation: CPU uses a FlagRegister it doesn't own or
                              // create -- it's passed in and can outlive/exist apart from this CPU
        Stack stack;
        unsigned char PC;
        unsigned char SI;

    public:
        // Initializes the CPU with PC and SI both at 0, aggregating an externally-owned FlagRegister.
        CPU(FlagRegister &fr) : flags(fr), PC(0), SI(0) {}

        // Returns a reference to general register i (0-7).
        GeneralRegister& getRegister(int i) { return registers[i]; }
        // Returns a reference to the CPU's flag register.
        FlagRegister& getFlags() { return flags; }
        // Returns a reference to the CPU's memory.
        Memory& getMemory() { return memory; }
        // Returns the current program counter.
        unsigned char getPC() { return PC; }
        // Advances the program counter by one instruction.
        void incrementPC() { PC++; }

        // Updates OF/UF/CF/ZF based on the unclamped result of an operation,
        // so overflow/underflow is detected before the value gets clamped.
        void updateFlags(int rawResult) {
            flags.setOF(rawResult > 127 ? 1 : 0);
            flags.setUF(rawResult < -128 ? 1 : 0);
            flags.setCF((rawResult > 127 || rawResult < -128)? 1 : 0);
            flags.setZF(rawResult == 0 ? 1 : 0);
        }

        // Returns the current stack index.
        unsigned char getSI() {return SI;}

        // Pushes value onto the stack and increments the stack index.
        void pushStack(int value) {
            stack.push(value);
            SI++;
        }

        // Pops the top of the stack and decrements the stack index.
        int popStack() {
            SI--;
            return stack.pop();

        }

        // Builds the dump format (registers/flags/PC/memory) into a single string.
        string buildDump() {
            ostringstream out;
            out << dec << "#Begin#" << endl;

            out << "#Registers";
            for (int i = 0; i < 8; i++) {
                out << "#" << setfill('0') << setw(4)
                    << (registers[i].getValue() & 0xFF);
            }
            out << "#" << endl;

            out << "#Flags#OF#" << flags.getOF()
                << "#UF#" << flags.getUF()
                << "#CF#" << flags.getCF()
                << "#ZF#" << flags.getZF() << "#" << endl;

            out << "#PC#" << setfill('0') << setw(4) << (int)PC << "#" << endl;

            out << "#Memory#" << endl;
            for (int row = 0; row < 8; row++){
                for(int col = 0; col < 8; col++) {
                    out << "#" << setfill('0') << setw(4)
                        << (memory.read(row * 8 + col) & 0xFF);
                }
                out << "#" << endl;
            }
            out << "#End#" << endl;
            return out.str();
        }

        // Prints the full VM state in the required output format to both the
        // screen and output.txt -- spec requires both ("store the results to a
        // file and display to the screen").
        void displayState() {
            string dump = buildDump();
            cout << dump;
            ofstream outFile("output.txt");
            outFile << dump;
        }
};

// Member 2: Adeeb
// Custom dynamic array used instead of std::vector to store the program's instructions.
template <typename T>
class MyVector {
    private:
        T* arr;
        int cap;
        int count;

        // Doubles capacity and copies existing elements into the new buffer.
        void grow() {
            cap = cap * 2;
            T* newArr = new T[cap];
            for (int i = 0; i < count; i++) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
        }

    public:
        // Starts with capacity 4 and no elements.
        MyVector() {
            cap = 4;
            count = 0;
            arr = new T[cap];
        }
        // Releases the backing array (does not delete pointed-to objects).
        ~MyVector() {
            delete[] arr;
        }

        // Appends val to the end, growing the backing array if needed.
        void push_back(T val) {
            if (count == cap) grow();
            arr[count] = val;
            count = count + 1;
        }

        // Returns the element at index i (no bounds checking).
        T get(int i) {
            return arr[i];
        }

        // Returns the number of elements currently stored.
        int size() {
            return count;
        }

        // Returns true if the vector has no elements.
        bool isEmpty() {
            return count == 0;
        }
};

// Member 2: Adeeb
// Abstract base for every opcode. Runner stores these polymorphically and calls execute().
class Instruction {
    public:
        // Virtual destructor so delete on a base Instruction* is well-defined.
        virtual ~Instruction() {}
        // Performs this instruction's effect on cpu; every opcode overrides this.
        virtual void execute(CPU &cpu) = 0;
};

// Member 2: Adeeb
// Intermediate abstract class grouping ADD/SUB/MUL/DIV/INC/DEC under one branch of the hierarchy.
class ArithmeticInstruction : public Instruction {
    public:
        // Still abstract; each arithmetic opcode supplies its own execute().
        virtual void execute(CPU &cpu) = 0;
};

// Member 2: Adeeb
// MOV Rdest, imm (mode 0) / MOV Rdest, Rsrc (mode 1) / MOV Rdest, [Rsrc] (mode 2).
class MovInstruction : public Instruction {
    private:
        int dest;
        int src;   // only used in mode 1 and 2
        int val;   // only used in mode 0
        int mode;

    public:
        // MOV Rdest, value (mode 0 = immediate). Pass mode explicitly as an int.
        MovInstruction(int d, int v, int m) {
            dest = d;
            val = v;
            src = -1;
            mode = m;
        }
        // MOV Rdest, Rsrc / MOV Rdest, [Rsrc]. Pass true for indirect, false for direct register mode.
        MovInstruction(int d, int s, bool indirect) {
            dest = d;
            src = s;
            val = 0;
            mode = indirect ? 2 : 1;
        }

        // Resolves the source value per mode, stores it into dest, then updates flags and PC.
        void execute(CPU &cpu) {
            int result;
            if (mode == 0) {
                result = val;
            } else if (mode == 1) {
                result = cpu.getRegister(src).getValue();
            } else {
                int addr = cpu.getRegister(src).getValue();
                result = cpu.getMemory().read(addr);
            }
            cpu.getRegister(dest).setValue(result);
            cpu.updateFlags(result);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// ADD Rdest, Rsrc or ADD Rdest, imm -> dest = dest + src. The spec's own worked
// example ("ADD R1, 6") uses an immediate despite section 3.5 saying both operands
// are registers, so both forms are supported, same as MovInstruction's modes.
class AddInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
        bool isImmediate;
    public:
        AddInstruction(int d, int s, bool isImm = false) { dest = d; src = s; isImmediate = isImm; }
        // Adds src (register or immediate) into dest, updates flags, advances PC.
        void execute(CPU &cpu) {
            int srcVal = isImmediate ? src : cpu.getRegister(src).getValue();
            int res = cpu.getRegister(dest).getValue() + srcVal;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// SUB Rdest, Rsrc or SUB Rdest, imm -> dest = dest - src.
class SubInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
        bool isImmediate;
    public:
        SubInstruction(int d, int s, bool isImm = false) { dest = d; src = s; isImmediate = isImm; }
        // Subtracts src (register or immediate) from dest, updates flags, advances PC.
        void execute(CPU &cpu) {
            int srcVal = isImmediate ? src : cpu.getRegister(src).getValue();
            int res = cpu.getRegister(dest).getValue() - srcVal;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// MUL Rdest, Rsrc or MUL Rdest, imm -> dest = dest * src.
class MulInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
        bool isImmediate;
    public:
        MulInstruction(int d, int s, bool isImm = false) { dest = d; src = s; isImmediate = isImm; }
        // Multiplies dest by src (register or immediate), updates flags, advances PC.
        void execute(CPU &cpu) {
            int srcVal = isImmediate ? src : cpu.getRegister(src).getValue();
            int res = cpu.getRegister(dest).getValue() * srcVal;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// DIV Rdest, Rsrc or DIV Rdest, imm -> dest = dest / src.
class DivInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
        bool isImmediate;
    public:
        DivInstruction(int d, int s, bool isImm = false) { dest = d; src = s; isImmediate = isImm; }
        // Divides dest by src (register or immediate) when src isn't 0
        // (leaving dest untouched otherwise), then advances PC.
        void execute(CPU &cpu) {
            int srcVal = isImmediate ? src : cpu.getRegister(src).getValue();
            if (srcVal != 0) {
                int res = cpu.getRegister(dest).getValue() / srcVal;
                cpu.getRegister(dest).setValue(res);
                cpu.updateFlags(res);
            }
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// INC Rdest -> dest = dest + 1
class IncInstruction : public ArithmeticInstruction {
    private:
        int dest;
    public:
        IncInstruction(int d) { dest = d; }
        // Increments dest, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() + 1;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// DEC Rdest -> dest = dest - 1
class DecInstruction : public ArithmeticInstruction {
    private:
        int dest;
    public:
        DecInstruction(int d) { dest = d; }
        // Decrements dest, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() - 1;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// Custom fixed-capacity circular queue used to hold the program's instructions.
// [DATA STRUCTURE: Circular Queue, array-based]
class MyQueue {
    private:
        Instruction* arr[128];
        int front, rear, count;
    public:
        // [OOP: Constructor]
        MyQueue() : front(0), rear(0), count(0) {}
        // Returns true if no instructions are queued.
        // [DATA STRUCTURE: queue empty-check]
        bool isEmpty() { return count == 0; }
        // Returns true if the queue has reached its 128-instruction capacity.
        // [DATA STRUCTURE: queue full-check]
        bool isFull() { return count == 128; }
        // Adds instr to the back of the queue if there's room.
        // [DATA STRUCTURE: enqueue operation, circular wraparound; OOP: Polymorphism - accepts any Instruction subtype via base pointer]
        void enqueue(Instruction* instr) {
            if (!isFull()) {
                arr[rear] = instr;
                rear = (rear + 1) % 128;
                count++;
            }
        }
        // Removes and returns the front instruction, or nullptr if empty.
        // [DATA STRUCTURE: dequeue operation, circular wraparound; OOP: Polymorphism - returns base-class pointer]
        Instruction* dequeue() {
            if (!isEmpty()) {
                Instruction* temp = arr[front];
                front = (front + 1) % 128;
                count--;
                return temp;
            }
            return nullptr;
        }
};

// Member 3: Ammar
// LOAD Rdest, [addr] (direct) or LOAD Rdest, [Rs] (indirect, addrOrReg names the
// register holding the address) -> copies a memory value into a register.
// [OOP: Inheritance - derives from Instruction]
class LoadInstruction : public Instruction {
    private:
        int dest, addrOrReg;
        bool indirect;
    public:
        // [OOP: Constructor]
        LoadInstruction(int d, int a, bool ind = false) : dest(d), addrOrReg(a), indirect(ind) {}
        // Resolves the address (direct or via register), reads memory, stores into dest.
        // [OOP: Polymorphism - overrides virtual Instruction::execute()]
        void execute(CPU &cpu) {
            int addr = indirect ? cpu.getRegister(addrOrReg).getValue() : addrOrReg;
            int val = cpu.getMemory().read(addr);
            cpu.getRegister(dest).setValue(val);
            cpu.updateFlags(val);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// STORE Rsrc, addr (direct) or STORE Rsrc, [Rd] (indirect, addrOrReg names the
// register holding the address) -> copies a register's value into memory.
// [OOP: Inheritance - derives from Instruction]
class StoreInstruction : public Instruction {
    private:
        int src, addrOrReg;
        bool indirect;
    public:
        // [OOP: Constructor]
        StoreInstruction(int s, int a, bool ind = false) : src(s), addrOrReg(a), indirect(ind) {}
        // Resolves the address (direct or via register), writes register src's value there.
        // [OOP: Polymorphism - overrides virtual Instruction::execute()]
        void execute(CPU &cpu) {
            int addr = indirect ? cpu.getRegister(addrOrReg).getValue() : addrOrReg;
            int val = cpu.getRegister(src).getValue();
            cpu.getMemory().write(addr, val);
            cpu.updateFlags(val);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// Intermediate abstract class grouping SHL/SHR/ROL/ROR under one branch of the hierarchy.
// [OOP: Abstraction + Inheritance - abstract intermediate class]
class ShiftInstruction : public Instruction {
    public:
        // Still abstract; each shift/rotate opcode supplies its own execute().
        // [OOP: Abstraction - pure virtual function makes this class abstract]
        virtual void execute(CPU &cpu) = 0;
};

// Member 3: Ammar
// ROL Rdest, count -> rotate dest's bits left by count positions (0-7).
// [OOP: Inheritance - derives from ShiftInstruction, two levels deep]
class RolInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        // [OOP: Constructor]
        RolInstruction(int d, int c) : dest(d), count(c) {}
        // Rotates dest's bit pattern left by count, reinterpreting the byte as signed
        // before storing so a rotate never gets clamped like an overflowed value would.
        // [OOP: Polymorphism - concrete override of pure virtual method]
        void execute(CPU &cpu) {
            unsigned char val = cpu.getRegister(dest).getValue();
            int n = ((count % 8) + 8) % 8;
            unsigned char res = (val << n) | (val >> (8 - n));
            cpu.getRegister(dest).setValue((signed char)res);
            cpu.updateFlags((signed char)res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// ROR Rdest, count -> rotate dest's bits right by count positions (0-7).
// [OOP: Inheritance - derives from ShiftInstruction]
class RorInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        // [OOP: Constructor]
        RorInstruction(int d, int c) : dest(d), count(c) {}
        // Rotates dest's bit pattern right by count; see RolInstruction for the signed-cast note.
        // [OOP: Polymorphism - overrides execute()]
        void execute(CPU &cpu) {
            unsigned char val = cpu.getRegister(dest).getValue();
            int n = ((count % 8) + 8) % 8;
            unsigned char res = (val >> n) | (val << (8 - n));
            cpu.getRegister(dest).setValue((signed char)res);
            cpu.updateFlags((signed char)res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// SHL Rdest, count -> logical left shift, zero-filling from the right. Shifting by
// 8 or more always yields 0 (every bit has been shifted out).
// [OOP: Inheritance - derives from ShiftInstruction]
class ShlInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        // [OOP: Constructor]
        ShlInstruction(int d, int c) : dest(d), count(c) {}
        // Shifts dest's unsigned byte pattern left by count (not the signed int value,
        // which would be UB/clamp incorrectly -- see RolInstruction), advances PC.
        // [OOP: Polymorphism - overrides execute()]
        void execute(CPU &cpu) {
            unsigned char val = cpu.getRegister(dest).getValue();
            unsigned char res = (count >= 8) ? 0 : (unsigned char)(val << count);
            cpu.getRegister(dest).setValue((signed char)res);
            cpu.updateFlags((signed char)res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// SHR Rdest, count -> logical right shift, zero-filling from the left. Shifting by
// 8 or more always yields 0.
// [OOP: Inheritance - derives from ShiftInstruction]
class ShrInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        // [OOP: Constructor]
        ShrInstruction(int d, int c) : dest(d), count(c) {}
        // Shifts dest's unsigned byte pattern right by count (a signed >> would sign-extend
        // instead of zero-filling, which the spec explicitly requires), advances PC.
        // [OOP: Polymorphism - overrides execute()]
        void execute(CPU &cpu) {
            unsigned char val = cpu.getRegister(dest).getValue();
            unsigned char res = (count >= 8) ? 0 : (unsigned char)(val >> count);
            cpu.getRegister(dest).setValue((signed char)res);
            cpu.updateFlags((signed char)res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// Intermediate abstract class grouping INPUT/DISPLAY under one branch of the hierarchy.
// [OOP: Abstraction + Inheritance - abstract intermediate class]
class IOInstruction : public Instruction {
    public:
        // Still abstract; INPUT and DISPLAY supply their own execute().
        // [OOP: Abstraction - pure virtual function makes this class abstract]
        virtual void execute(CPU &cpu) = 0;
};

// Member 3: Ammar
// INPUT Rdest -> reads a value from the user into a register.
// [OOP: Inheritance - derives from IOInstruction, two levels deep]
class InputInstruction : public IOInstruction {
    private:
        int dest;
    public:
        // [OOP: Constructor]
        InputInstruction(int d) : dest(d) {}
        // Prompts with "?" on a new line (per spec), stores the value in dest,
        // updates flags, advances PC.
        // [OOP: Polymorphism - concrete override of pure virtual method]
        void execute(CPU &cpu) {
            int val;
            cout << endl << "?";
            cin >> val;
            cpu.getRegister(dest).setValue(val);
            cpu.updateFlags(val);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// DISPLAY Rsrc -> prints a register's value to the console.
// [OOP: Inheritance - derives from IOInstruction]
class DisplayInstruction : public IOInstruction {
    private:
        int src;
    public:
        // [OOP: Constructor]
        DisplayInstruction(int s) : src(s) {}
        // Prints register src's value, advances PC.
        // [OOP: Polymorphism - concrete override of pure virtual method]
        void execute(CPU &cpu) {
            cout << "DISPLAY R" << src << " = " << cpu.getRegister(src).getValue() << endl;
            cpu.incrementPC();
        }
};

// Member 1: Adam
// OOP: Inheritance (derives from abstract Instruction) + Polymorphism
// (execute() overridden and invoked through an Instruction* in the program's
// instruction vector). No data structure interaction.
// RESET <CF|OF|UF|ZF> -> clears exactly one flag back to 0.
class ResetInstruction : public Instruction {
    private:
        char flag; // 'O'=OF, 'U'=UF, 'C'=CF, 'Z'=ZF
    public:
        ResetInstruction(char f) : flag(f) {}
        // Clears the selected flag on the CPU's FlagRegister, advances PC.
        void execute(CPU &cpu) {
            FlagRegister &flags = cpu.getFlags();
            if (flag == 'O') flags.setOF(0);
            else if (flag == 'U') flags.setUF(0);
            else if (flag == 'C') flags.setCF(0);
            else if (flag == 'Z') flags.setZF(0);
            cpu.incrementPC();
        }
};

// Member 1: Adam
// Both: OOP - Inheritance + Polymorphism (as above). Data Structure - drives
// the Stack via cpu.pushStack().
// PUSH Rsrc -> pushes register src's value onto the stack.
class PushInstruction : public Instruction {
    private:
        int src;
    public:
        PushInstruction(int s) : src(s) {}
        // Pushes register src's value onto the CPU's stack, advances PC.
        void execute(CPU &cpu) {
            cpu.pushStack(cpu.getRegister(src).getValue());
            cpu.incrementPC();
        }
};

// Member 1: Adam
// Both: OOP - Inheritance + Polymorphism (as above). Data Structure - drives
// the Stack via cpu.popStack().
// POP Rdest -> pops the top of the stack into register dest.
class PopInstruction : public Instruction {
    private:
        int dest;
    public:
        PopInstruction(int d) : dest(d) {}
        // Pops the stack into register dest, updates flags, advances PC.
        void execute(CPU &cpu) {
            int val = cpu.popStack();
            cpu.getRegister(dest).setValue(val);
            cpu.updateFlags(val);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// .asm parsing helpers. Lines look like "MOV R0, 5" / "LOAD R1, [10]" / "LOAD R1, [R2]" /
// "; a comment". Anything after a ';' is ignored, as are blank lines.
// [Note: these are free functions, not OOP - simple procedural string helpers]

// Strips leading/trailing whitespace from s.
static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Returns true if tok is wrapped in [ ] (used for indirect addressing operands).
static bool isBracketed(const string &tok) {
    return tok.size() >= 2 && tok.front() == '[' && tok.back() == ']';
}

// Strips the [ ] wrapper from tok and trims the inside, or returns tok unchanged.
static string stripBrackets(const string &tok) {
    if (isBracketed(tok)) return trim(tok.substr(1, tok.size() - 2));
    return tok;
}

// Parses "R3" -> 3, or -1 if tok isn't a valid R0-R7 register name.
static int parseRegister(const string &tok) {
    if (tok.size() < 2 || (tok[0] != 'R' && tok[0] != 'r')) return -1;
    for (size_t i = 1; i < tok.size(); i++) {
        if (!isdigit((unsigned char)tok[i])) return -1;
    }
    int id = stoi(tok.substr(1));
    return (id >= 0 && id <= 7) ? id : -1;
}

// Builds the Instruction for MOV from its two operand tokens (handles all 3 addressing modes).
static Instruction* buildMov(const string &op1, const string &op2) {
    int dest = parseRegister(op1);
    if (isBracketed(op2)) {
        return new MovInstruction(dest, parseRegister(stripBrackets(op2)), true);
    }
    int srcReg = parseRegister(op2);
    if (srcReg != -1) return new MovInstruction(dest, srcReg, false);
    return new MovInstruction(dest, stoi(op2), 0);
}

// Builds the Instruction for ADD/SUB/MUL/DIV/INC/DEC from the opcode and its operands.
// The second operand may be a register (e.g. "ADD R0, R1") or an immediate (e.g.
// "ADD R1, 6", as used in the spec's own worked example) -- whichever it parses as.
static Instruction* buildArithmetic(const string &op, const string &op1, const string &op2) {
    int dest = parseRegister(op1);
    if (op == "INC") return new IncInstruction(dest);
    if (op == "DEC") return new DecInstruction(dest);
    int srcReg = parseRegister(op2);
    bool isImm = (srcReg == -1);
    int src = isImm ? stoi(op2) : srcReg;
    if (op == "ADD") return new AddInstruction(dest, src, isImm);
    if (op == "SUB") return new SubInstruction(dest, src, isImm);
    if (op == "MUL") return new MulInstruction(dest, src, isImm);
    return new DivInstruction(dest, src, isImm);
}

// Builds the Instruction for LOAD/STORE. LOAD's destination register is always first
// ("LOAD Rd, [addr]"/"LOAD Rd, [Rs]"). STORE's operand order is inconsistent in the
// spec itself -- section 3.9 shows "STORE Rs, addr"/"STORE Rs, [Rd]" (register first)
// but the spec's own end-to-end worked example shows "STORE addr, Rs" (address
// first) -- so for STORE we detect which operand is the register regardless of
// position, instead of assuming an order.
static Instruction* buildMemory(const string &op, const string &op1, const string &op2) {
    if (op == "LOAD") {
        int dest = parseRegister(op1);
        string inner = isBracketed(op2) ? stripBrackets(op2) : op2;
        int addrReg = parseRegister(inner);
        if (addrReg != -1) return new LoadInstruction(dest, addrReg, true);
        return new LoadInstruction(dest, stoi(inner), false);
    }
    int reg1 = parseRegister(op1);
    if (reg1 != -1) {
        string inner = isBracketed(op2) ? stripBrackets(op2) : op2;
        int reg2 = parseRegister(inner);
        if (reg2 != -1) return new StoreInstruction(reg1, reg2, true);
        return new StoreInstruction(reg1, stoi(inner), false);
    }
    return new StoreInstruction(parseRegister(op2), stoi(op1), false);
}

// Builds the Instruction for ROL/ROR/SHL/SHR from the opcode, destination register, and count.
static Instruction* buildShift(const string &op, const string &op1, const string &op2) {
    int dest = parseRegister(op1);
    int count = stoi(op2);
    if (op == "ROL") return new RolInstruction(dest, count);
    if (op == "ROR") return new RorInstruction(dest, count);
    if (op == "SHL") return new ShlInstruction(dest, count);
    return new ShrInstruction(dest, count);
}

// Builds the Instruction for INPUT/DISPLAY from the opcode and its single register operand.
static Instruction* buildIO(const string &op, const string &op1) {
    int reg = parseRegister(op1);
    return (op == "INPUT") ? (Instruction*)new InputInstruction(reg)
                            : (Instruction*)new DisplayInstruction(reg);
}

// Builds the Instruction for RESET from its single flag-name operand (OF/UF/CF/ZF).
static Instruction* buildReset(const string &op1) {
    string f = op1;
    for (auto &c : f) c = toupper((unsigned char)c);
    if (f == "OF") return new ResetInstruction('O');
    if (f == "UF") return new ResetInstruction('U');
    if (f == "CF") return new ResetInstruction('C');
    if (f == "ZF") return new ResetInstruction('Z');
    return nullptr;
}

// Builds the Instruction for PUSH/POP from the opcode and its single register operand.
static Instruction* buildStack(const string &op, const string &op1) {
    int reg = parseRegister(op1);
    return (op == "PUSH") ? (Instruction*)new PushInstruction(reg)
                           : (Instruction*)new PopInstruction(reg);
}

// Returns true if tok still has internal whitespace after trimming -- a valid single
// operand (register/immediate/[addr]/flag name) never does, so this catches stray
// leftover text, e.g. a second instruction crammed onto the same line.
static bool hasInternalSpace(const string &tok) {
    return tok.find(' ') != string::npos || tok.find('\t') != string::npos;
}

// Parses one .asm line into the matching Instruction, or nullptr for blank/comment/unknown lines.
// Exits with an error if more than one instruction is found on the line (per spec).
static Instruction* parseLine(const string &rawLine) {
    string line = rawLine;
    size_t semi = line.find(';');
    if (semi != string::npos) line = line.substr(0, semi);
    line = trim(line);
    if (line.empty()) return nullptr;

    size_t sp = line.find(' ');
    string opcode = (sp == string::npos) ? line : line.substr(0, sp);
    string rest = (sp == string::npos) ? "" : trim(line.substr(sp + 1));
    for (auto &c : opcode) c = toupper((unsigned char)c);

    size_t comma = rest.find(',');
    string op1 = trim(comma == string::npos ? rest : rest.substr(0, comma));
    string op2 = (comma == string::npos) ? "" : trim(rest.substr(comma + 1));

    if (hasInternalSpace(op1) || hasInternalSpace(op2)) {
        cerr << "ERROR: more than one instruction found on one line: " << rawLine << endl;
        exit(1);
    }

    if (opcode == "MOV") return buildMov(op1, op2);
    if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL" ||
        opcode == "DIV" || opcode == "INC" || opcode == "DEC") {
        return buildArithmetic(opcode, op1, op2);
    }
    if (opcode == "LOAD" || opcode == "STORE") return buildMemory(opcode, op1, op2);
    if (opcode == "ROL" || opcode == "ROR" || opcode == "SHL" || opcode == "SHR") {
        return buildShift(opcode, op1, op2);
    }
    if (opcode == "INPUT" || opcode == "DISPLAY") return buildIO(opcode, op1);
    if (opcode == "RESET") return buildReset(op1);
    if (opcode == "PUSH" || opcode == "POP") return buildStack(opcode, op1);
    return nullptr;
}

// Member 3: Ammar
// Drives a queued program: reads a .asm file, parses each line into an Instruction,
// enqueues it, then executes the queue in order via the CPU.
// [OOP: Class combining Composition - owns a MyQueue and a CPU]
class Runner {
    private:
        MyQueue instrQueue;
        FlagRegister flags; // owned here, aggregated by cpu below (declared after, per init order)
        CPU cpu;
    public:
        // Constructs cpu aggregating this Runner's FlagRegister.
        // [OOP: Constructor with member-initializer list]
        Runner() : cpu(flags) {}

        // Reads filename into a MyVector<string> (one element per line, per spec), then
        // parses each stored line into an Instruction and enqueues it for execution.
        // [DATA STRUCTURE: uses MyVector (dynamic array) and MyQueue (circular queue); OOP: Polymorphism - parseLine returns base-class Instruction* for any opcode]
        void loadProgram(const string &filename) {
            MyVector<string> lines;
            ifstream file(filename);
            if (!file.is_open()) {
                cerr << "Error: could not open file \"" << filename << "\"" << endl;
                exit(1);
            }
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            for (int i = 0; i < lines.size(); i++) {
                Instruction* instr = parseLine(lines.get(i));
                if (instr != nullptr) instrQueue.enqueue(instr);
            }
        }
        // Dequeues and executes every instruction in order. If stepMode is true, the
        // full VM state is dumped after every single instruction (for report
        // screenshots); otherwise only the final state is dumped, per spec.
        // [DATA STRUCTURE: drains the MyQueue via dequeue/isEmpty; OOP: Polymorphism - instr->execute(cpu) dispatches to the correct override at runtime]
        void run(bool stepMode = false) {
            while (!instrQueue.isEmpty()) {
                Instruction* instr = instrQueue.dequeue();
                instr->execute(cpu);
                delete instr;
                if (stepMode) cpu.displayState();
            }
            if (!stepMode) cpu.displayState();
        }
};

// Demonstrates polymorphic dispatch through MyVector<Instruction*> (a second, separate
// site from Runner's queue-based dispatch) -- virtual execute() resolves correctly
// through the base Instruction* regardless of which concrete subclass it points to.
void demoPolymorphism() {
    cout << "===Polymorphism Demo (MyVector<Instruction*>)===" << endl;
    FlagRegister flags;
    CPU cpu(flags);
    MyVector<Instruction*> program;
    program.push_back(new MovInstruction(1, 5, 0)); // MOV R1, 5
    program.push_back(new AddInstruction(1, 1));    // ADD R1, R1
    program.push_back(new IncInstruction(1));       // INC R1
    for (int i = 0; i < program.size(); i++) {
        program.get(i)->execute(cpu);
        cout << "after instruction " << i << " -> R1=" << cpu.getRegister(1).getValue() << endl;
    }
    for (int i = 0; i < program.size(); i++) {
        delete program.get(i);
    }
}

// Entry point: loads the assembly program named on the command line (examiners can
// pass their own .asm file here), defaulting to test_program.asm if none is given.
// A second argument "--step" dumps the full VM state after every instruction
// instead of just at the end (useful for report screenshots).
int main(int argc, char* argv[]) {
    demoPolymorphism();
    cout << "\n";
    string filename = (argc > 1) ? argv[1] : "test_program.asm";
    bool stepMode = (argc > 2 && string(argv[2]) == "--step");
    Runner runner;
    runner.loadProgram(filename);
    runner.run(stepMode);
    return 0;
}
