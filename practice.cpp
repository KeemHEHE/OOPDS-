#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// Member 1: Adam
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
//
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

        // Pops and returns the top value, or 0 if the stack is empty.
        int pop() {
            if (!isEmpty()) {
                int val = data[top];
                top--;
                return val;
            }
            return 0;
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
class CPU {
    private:
        GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
        Memory memory;
        FlagRegister flags;
        Stack stack;
        unsigned char PC;
        unsigned char SI;

    public:
        // Initializes the CPU with PC and SI both at 0.
        CPU() : PC(0), SI(0) {}

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

        // Prints the full VM state (registers, flags, PC, memory) in the required output format.
        void displayState() {
            cout << "#Begin#" << endl;

            cout << "#Registers";
            for (int i = 0; i < 8; i++) {
                cout << "#" << setfill('0') << setw(4) << hex
                     << (registers[i].getValue() & 0xFF);
            }
            cout << "#" << endl;

            cout << "#Flags#OF#" << dec << flags.getOF()
                 << "#UF#" << flags.getUF()
                 << "#CF#" << flags.getCF()
                 << "#ZF#" << flags.getZF() << "#" << endl;

            cout << "#PC#" << setfill('0') << setw(4) << hex
                 << (int)PC << "#" << endl;

            cout << "#Memory#" << endl;
            for (int row = 0; row < 8; row++){
                for(int col = 0; col < 8; col++) {
                    cout << "#" << setfill('0') << setw(4) << hex
                        <<(memory.read(row * 8 + col) & 0xFF);

                }
                cout << "#" << endl;
            }

            cout << "#End#" << endl;




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
// ADD Rdest, Rsrc -> dest = dest + src
class AddInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
    public:
        AddInstruction(int d, int s) { dest = d; src = s; }
        // Adds src into dest, updates flags from the raw (unclamped) result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() + cpu.getRegister(src).getValue();
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// SUB Rdest, Rsrc -> dest = dest - src
class SubInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
    public:
        SubInstruction(int d, int s) { dest = d; src = s; }
        // Subtracts src from dest, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() - cpu.getRegister(src).getValue();
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// MUL Rdest, Rsrc -> dest = dest * src
class MulInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
    public:
        MulInstruction(int d, int s) { dest = d; src = s; }
        // Multiplies dest by src, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() * cpu.getRegister(src).getValue();
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 2: Adeeb
// DIV Rdest, Rsrc -> dest = dest / src
class DivInstruction : public ArithmeticInstruction {
    private:
        int dest, src;
    public:
        DivInstruction(int d, int s) { dest = d; src = s; }
        // Divides dest by src when src isn't 0 (leaving dest untouched otherwise), then advances PC.
        void execute(CPU &cpu) {
            int srcVal = cpu.getRegister(src).getValue();
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
class MyQueue {
    private:
        Instruction* arr[128];
        int front, rear, count;
    public:
        MyQueue() : front(0), rear(0), count(0) {}
        // Returns true if no instructions are queued.
        bool isEmpty() { return count == 0; }
        // Returns true if the queue has reached its 128-instruction capacity.
        bool isFull() { return count == 128; }
        // Adds instr to the back of the queue if there's room.
        void enqueue(Instruction* instr) {
            if (!isFull()) {
                arr[rear] = instr;
                rear = (rear + 1) % 128;
                count++;
            }
        }
        // Removes and returns the front instruction, or nullptr if empty.
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
class LoadInstruction : public Instruction {
    private:
        int dest, addrOrReg;
        bool indirect;
    public:
        LoadInstruction(int d, int a, bool ind = false) : dest(d), addrOrReg(a), indirect(ind) {}
        // Resolves the address (direct or via register), reads memory, stores into dest.
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
class StoreInstruction : public Instruction {
    private:
        int src, addrOrReg;
        bool indirect;
    public:
        StoreInstruction(int s, int a, bool ind = false) : src(s), addrOrReg(a), indirect(ind) {}
        // Resolves the address (direct or via register), writes register src's value there.
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
class ShiftInstruction : public Instruction {
    public:
        // Still abstract; each shift/rotate opcode supplies its own execute().
        virtual void execute(CPU &cpu) = 0;
};

// Member 3: Ammar
// ROL Rdest, count -> rotate dest's bits left by count positions (0-7).
class RolInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        RolInstruction(int d, int c) : dest(d), count(c) {}
        // Rotates dest's bit pattern left by count, reinterpreting the byte as signed
        // before storing so a rotate never gets clamped like an overflowed value would.
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
class RorInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        RorInstruction(int d, int c) : dest(d), count(c) {}
        // Rotates dest's bit pattern right by count; see RolInstruction for the signed-cast note.
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
// SHL Rdest, count -> shift dest's bits left by count positions.
class ShlInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        ShlInstruction(int d, int c) : dest(d), count(c) {}
        // Shifts dest left by count, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() << count;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// SHR Rdest, count -> shift dest's bits right by count positions.
class ShrInstruction : public ShiftInstruction {
    private:
        int dest, count;
    public:
        ShrInstruction(int d, int c) : dest(d), count(c) {}
        // Shifts dest right by count, updates flags from the raw result, advances PC.
        void execute(CPU &cpu) {
            int res = cpu.getRegister(dest).getValue() >> count;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// Intermediate abstract class grouping INPUT/DISPLAY under one branch of the hierarchy.
class IOInstruction : public Instruction {
    public:
        // Still abstract; INPUT and DISPLAY supply their own execute().
        virtual void execute(CPU &cpu) = 0;
};

// Member 3: Ammar
// INPUT Rdest -> reads a value from the user into a register.
class InputInstruction : public IOInstruction {
    private:
        int dest;
    public:
        InputInstruction(int d) : dest(d) {}
        // Prompts for a value, stores it in dest, updates flags, advances PC.
        void execute(CPU &cpu) {
            int val;
            cout << "Enter value: ";
            cin >> val;
            cpu.getRegister(dest).setValue(val);
            cpu.updateFlags(val);
            cpu.incrementPC();
        }
};

// Member 3: Ammar
// DISPLAY Rsrc -> prints a register's value to the console.
class DisplayInstruction : public IOInstruction {
    private:
        int src;
    public:
        DisplayInstruction(int s) : src(s) {}
        // Prints register src's value, advances PC.
        void execute(CPU &cpu) {
            cout << "DISPLAY R" << src << " = " << cpu.getRegister(src).getValue() << endl;
            cpu.incrementPC();
        }
};

// Member 1: Adam
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
static Instruction* buildArithmetic(const string &op, const string &op1, const string &op2) {
    int dest = parseRegister(op1);
    if (op == "INC") return new IncInstruction(dest);
    if (op == "DEC") return new DecInstruction(dest);
    int src = parseRegister(op2);
    if (op == "ADD") return new AddInstruction(dest, src);
    if (op == "SUB") return new SubInstruction(dest, src);
    if (op == "MUL") return new MulInstruction(dest, src);
    return new DivInstruction(dest, src);
}

// Builds the Instruction for LOAD/STORE. LOAD always brackets its operand ([addr] or
// [Rs]); STORE only brackets the indirect form. Either way, what matters is whether
// the bracketed (or bare) token names a register or a literal address.
static Instruction* buildMemory(const string &op, const string &op1, const string &op2) {
    int reg = parseRegister(op1);
    string inner = isBracketed(op2) ? stripBrackets(op2) : op2;
    int addrReg = parseRegister(inner);
    if (addrReg != -1) {
        return (op == "LOAD") ? (Instruction*)new LoadInstruction(reg, addrReg, true)
                               : (Instruction*)new StoreInstruction(reg, addrReg, true);
    }
    int addr = stoi(inner);
    return (op == "LOAD") ? (Instruction*)new LoadInstruction(reg, addr, false)
                           : (Instruction*)new StoreInstruction(reg, addr, false);
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

// Parses one .asm line into the matching Instruction, or nullptr for blank/comment/unknown lines.
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
class Runner {
    private:
        MyQueue instrQueue;
        CPU cpu;
    public:
        // Reads filename line by line and enqueues the instruction parsed from each line.
        void loadProgram(const string &filename) {
            ifstream file(filename);
            string line;
            while (getline(file, line)) {
                Instruction* instr = parseLine(line);
                if (instr != nullptr) instrQueue.enqueue(instr);
            }
        }
        // Dequeues and executes every instruction in order, then prints the final CPU state.
        void run() {
            while (!instrQueue.isEmpty()) {
                Instruction* instr = instrQueue.dequeue();
                instr->execute(cpu);
                delete instr;
            }
            cpu.displayState();
        }
};

// Demonstrates polymorphic dispatch through MyVector<Instruction*> (a second, separate
// site from Runner's queue-based dispatch) -- virtual execute() resolves correctly
// through the base Instruction* regardless of which concrete subclass it points to.
void demoPolymorphism() {
    cout << "===Polymorphism Demo (MyVector<Instruction*>)===" << endl;
    CPU cpu;
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

// Entry point: loads the assembly program in test_program.asm and runs it through the VM.
int main() {
    demoPolymorphism();
    cout << "\n";
    Runner runner;
    runner.loadProgram("test_program.asm");
    runner.run();
    return 0;
}
