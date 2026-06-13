#include <iostream>
using namespace std;

// Member 1: Adam
class Register {
    private:
        signed char value;

    public:
        Register() {
            value = 0;
        }
        virtual ~Register() {}

        void setValue(int n) {
            if (n > 127) {
                value = 127;
            } else if (n < -128) {
                value = -128;
            } else {
                value = n;
            }
        }

        int getValue() {
            return value;
        }

        void display() {
            cout << "Register value: " << (int)value << endl;
        }
};

// Member 1: Adam
class GeneralRegister : public Register {
    private:
        int id;
    public:
        GeneralRegister() : id(0) {}
        GeneralRegister(int id) : id(id) {}
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
        FlagRegister() {
            OF = 0;
            UF = 0;
            CF = 0;
            ZF = 0;
        }

        void setOF(int v) { OF = v; }
        void setUF(int v) { UF = v; }
        void setCF(int v) { CF = v; }
        void setZF(int v) { ZF = v; }

        int getOF() { return OF; }
        int getUF() { return UF; }
        int getCF() { return CF; }
        int getZF() { return ZF; }

        void displayFlags() {
            cout << "OF=" << OF << " UF=" << UF << " CF=" << CF << " ZF=" << ZF << endl;
        }

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
        Memory() {
            for (int i = 0; i < 64; i++) {
                data[i] = 0;
            }
        }

        void write(int address, int value) {
            if (address >= 0 && address <= 63) {
                data[address] = value;
            }
        }

        int read(int address) {
            if (address >= 0 && address <= 63) {
                return data[address];
            }
            return 0;
        }
};

// Member 1: Adam
class CPU {
    private:
        GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
        Memory memory;
        FlagRegister flags;
        unsigned char PC;
        unsigned char SI;

    public:
        CPU() : PC(0), SI(0) {}

        GeneralRegister& getRegister(int i) { return registers[i]; }
        FlagRegister& getFlags() { return flags; }
        Memory& getMemory() { return memory; }
        unsigned char getPC() { return PC; }
        void incrementPC() { PC++; }
};

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

    return 0;
}
