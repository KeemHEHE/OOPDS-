#include <iostream>
#include <iomanip>
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

// Runs quick smoke tests for every Member 1 class (Register, GeneralRegister,
// FlagRegister, Memory, Stack, CPU) and prints the results to the console.
int main() {

    // Test Register
    cout << "===Register Tests===" << endl;
    Register R0;
    R0.setValue(50);
    cout << "R0 value: " << R0.getValue() << endl;
    R0.setValue(200);
    cout << "R0 max clamp: " << R0.getValue() << endl;
    R0.setValue(-200);
    cout << "R0 min clamp: " << R0.getValue() << endl;
    R0.setValue(42);
    R0.display();

    // Test GeneralRegister array
    cout << "\n===GeneralRegister Tests===" << endl;
    GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
    registers[0].setValue(5);
    registers[1].setValue(10);
    registers[3].setValue(200);
    for (int i = 0; i < 8; i++) {
        cout << "R" << i << " = " << registers[i].getValue() << endl;
    }

    // Test FlagRegister
    cout << "\n===FlagRegister Tests===" << endl;
    FlagRegister flags;
    flags.displayFlags();
    flags.setOF(1);
    flags.displayFlags();
    flags.reset();
    flags.setZF(1);
    flags.displayFlags();

    // Test Memory
    cout << "\n===Memory Tests===" << endl;
    Memory mem;
    mem.write(10, 99);
    cout << "mem[10] = " << mem.read(10) << endl;
    cout << "mem[0]  = " << mem.read(0) << endl;

    // Test CPU
    cout << "\n===CPU Tests===" << endl;
    CPU cpu;
    cpu.getRegister(0).setValue(77);
    cout << "CPU R0 = " << cpu.getRegister(0).getValue() << endl;
    cpu.getMemory().write(5, 33);
    cout << "CPU mem[5] = " << cpu.getMemory().read(5) << endl;
    cpu.incrementPC();
    cout << "CPU PC = " << (int)cpu.getPC() << endl;

    // Test displayState
    cout << "\n===displayState Test===" << endl;
    CPU cpu2;
    cpu2.getRegister(1).setValue(17);
    cpu2.getRegister(3).setValue(68);
    cpu2.incrementPC();
    cpu2.incrementPC();
    cpu2.getMemory().write(20, 68);
    cpu2.displayState();

    // Test Instruction polymorphism (Member 2: Adeeb's classes, integrated here)
    cout << dec; // displayState() leaves cout in hex mode, switch back for normal numbers
    cout << "\n===Instruction Tests===" << endl;
    CPU cpu3;
    MyVector<Instruction*> program;
    program.push_back(new MovInstruction(1, 5, 0));   // MOV R1, 5
    program.push_back(new AddInstruction(1, 1));      // ADD R1, R1
    program.push_back(new MulInstruction(1, 1));       // MUL R1, R1
    program.push_back(new IncInstruction(1));          // INC R1
    for (int i = 0; i < program.size(); i++) {
        Instruction* instr = program.get(i);
        instr->execute(cpu3);
        cout << "after instruction " << i << " -> R1=" << cpu3.getRegister(1).getValue()
             << " PC=" << (int)cpu3.getPC() << endl;
    }
    for (int i = 0; i < program.size(); i++) {
        delete program.get(i);
    }

    return 0;
}
