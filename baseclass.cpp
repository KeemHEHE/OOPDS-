#include <iostream>
#include <iomanip>
using namespace std;

// =======================
// Member 1: Adam
// =======================
class Register {
private:
    signed char value;
public:
    Register() { value = 0; }
    virtual ~Register() {}
    void setValue(int n) {
        if (n > 127) value = 127;
        else if (n < -128) value = -128;
        else value = n;
    }
    int getValue() { return value; }
    void display() { cout << "Register value: " << (int)value << endl; }
};

class GeneralRegister : public Register {
private:
    int id;
public:
    GeneralRegister() : id(0) {}
    GeneralRegister(int id) : id(id) {}
    int getID() { return id; }
};

class FlagRegister {
private:
    int OF, UF, CF, ZF;
public:
    FlagRegister() { OF=UF=CF=ZF=0; }
    void setOF(int v) { OF=v; }
    void setUF(int v) { UF=v; }
    void setCF(int v) { CF=v; }
    void setZF(int v) { ZF=v; }
    int getOF() { return OF; }
    int getUF() { return UF; }
    int getCF() { return CF; }
    int getZF() { return ZF; }
    void displayFlags() {
        cout << "OF="<<OF<<" UF="<<UF<<" CF="<<CF<<" ZF="<<ZF<<endl;
    }
    void reset() { OF=UF=CF=ZF=0; }
};

class Memory {
private:
    signed char data[64];
public:
    Memory() { for(int i=0;i<64;i++) data[i]=0; }
    void write(int address,int value) {
        if(address>=0 && address<=63) data[address]=value;
    }
    int read(int address) {
        if(address>=0 && address<=63) return data[address];
        return 0;
    }
};

class Stack {
private:
    signed char data[64];
    int top;
public:
    Stack(): top(-1) {}
    bool isEmpty(){ return top==-1; }
    bool isFull(){ return top==63; }
    void push(int value){
        if(!isFull()){ top++; data[top]=(signed char)value; }
    }
    int pop(){
        if(!isEmpty()){ int val=data[top]; top--; return val; }
        return 0;
    }
    int peek(){ return (!isEmpty())?data[top]:0; }
    int getTop(){ return top; }
};

class CPU {
private:
    GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
    Memory memory;
    FlagRegister flags;
    Stack stack;
    unsigned char PC;
    unsigned char SI;
public:
    CPU(): PC(0), SI(0) {}
    GeneralRegister& getRegister(int i){ return registers[i]; }
    FlagRegister& getFlags(){ return flags; }
    Memory& getMemory(){ return memory; }
    unsigned char getPC(){ return PC; }
    void incrementPC(){ PC++; }
    void updateFlags(int rawResult){
        flags.setOF(rawResult>127?1:0);
        flags.setUF(rawResult<-128?1:0);
        flags.setCF((rawResult>127||rawResult<-128)?1:0);
        flags.setZF(rawResult==0?1:0);
    }
    unsigned char getSI(){ return SI; }
    void pushStack(int value){ stack.push(value); SI++; }
    int popStack(){ SI--; return stack.pop(); }
    void displayState(){
        cout<<"#Begin#"<<endl;
        cout<<"#Registers";
        for(int i=0;i<8;i++){
            cout<<"#"<<setfill('0')<<setw(4)<<hex<<(registers[i].getValue()&0xFF);
        }
        cout<<"#"<<endl;
        cout<<"#Flags#OF#"<<dec<<flags.getOF()
            <<"#UF#"<<flags.getUF()
            <<"#CF#"<<flags.getCF()
            <<"#ZF#"<<flags.getZF()<<"#"<<endl;
        cout<<"#PC#"<<setfill('0')<<setw(4)<<hex<<(int)PC<<"#"<<endl;
        cout<<"#Memory#"<<endl;
        for(int row=0;row<8;row++){
            for(int col=0;col<8;col++){
                cout<<"#"<<setfill('0')<<setw(4)<<hex<<(memory.read(row*8+col)&0xFF);
            }
            cout<<"#"<<endl;
        }
        cout<<"#End#"<<endl;
    }
};

// =======================
// Member 2: Adeeb
// =======================

template <typename T>
class MyVector{
    private:
    T* arr;
    int cap;
    int count;

    void grow(){
        cap = cap*2;
        T* newArr = new T[cap];
        for(int i=0; i<count; i++){
            newArr[i] = arr[i];
        }
        delete [] arr;
        arr = newArr;
    }

    public:
    MyVector(){
        cap=4;
        count=0;
        arr = new T[cap];
    }
    ~MyVector(){
        delete[] arr;
    }

    void push_back(T val){
        if(count==cap) grow();
        arr[count]=val;
        count=count+1;
    }

    T get(int i){
        return arr[i]; //no bounds checking, just keep it simple for now
    }

    int size(){
        return count;
    }

    bool isEmpty(){
        if(count==0) return true;
        return false;
    }
};


// instruction base class, everything below this must override execute()
class Instruction{
    public:
    virtual ~Instruction(){} //without this delete on the base pointer is UB, found out after testing
    virtual void execute(CPU &cpu)=0;
};

class MovInstruction : public Instruction{
    private:
    int dest;
    int src;   //only matters for mode 1 and 2
    int val;   //only matters for mode 0
    int mode;

    public:
    //this constructor is for MOV Rdest, value  (mode 0)
    MovInstruction(int d, int v, int m){
        dest=d;
        val=v;
        src=-1;
        mode=m;
    }
    //this one is for MOV Rdest, Rsrc and MOV Rdest, [Rsrc]
    //pass true for indirect mode, false for normal register mode
    MovInstruction(int d, int s, bool indirect){
        dest=d;
        src=s;
        val=0;
        if(indirect==true){
            mode=2;
        }else{
            mode=1;
        }
    }

    void execute(CPU &cpu){
        int result;
        if(mode==0){
            result = val;
        }
        else if(mode==1){
            result = cpu.getRegister(src).getValue();
        }
        else{
            //mode 2, indirect. src register actually holds a memory address
            int addr = cpu.getRegister(src).getValue();
            result = cpu.getMemory().read(addr);
        }
        cpu.getRegister(dest).setValue(result);
        cpu.updateFlags(result);
        cpu.incrementPC();
    }
};


// ADD Rdest, Rsrc
// dest = dest + src
class AddInstruction : public Instruction {
private:
    int dest,src;
public:
    AddInstruction(int d, int s){
        dest=d;
        src=s;
    }
    void execute(CPU &cpu){
        int res = cpu.getRegister(dest).getValue() + cpu.getRegister(src).getValue();
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res); // update overflow/zero flag based on raw result
        cpu.incrementPC();
    }
};


// SUB Rdest, Rsrc
// dest = dest - src
class SubInstruction : public Instruction {
private:
    int dest, src;
public:
    SubInstruction(int d, int s) { dest = d; src = s; }
    void execute(CPU &cpu) {
        int res = cpu.getRegister(dest).getValue() - cpu.getRegister(src).getValue();
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// MUL Rdest, Rsrc
// dest = dest * src
class MulInstruction : public Instruction {
private:
    int dest, src;
public:
    MulInstruction(int d, int s) { dest = d; src = s; }
    void execute(CPU &cpu) {
        int res = cpu.getRegister(dest).getValue() * cpu.getRegister(src).getValue();
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// DIV Rdest, Rsrc
// dest = dest / src
class DivInstruction : public Instruction {
private:
    int dest, src;
public:
    DivInstruction(int d, int s) { dest = d; src = s; }
    void execute(CPU &cpu) {
        int srcVal = cpu.getRegister(src).getValue();
        if (srcVal != 0) {
            int res = cpu.getRegister(dest).getValue() / srcVal;
            cpu.getRegister(dest).setValue(res);
            cpu.updateFlags(res);
        }
        // if srcVal == 0 we still move on to next instruction, just dont touch dest
        cpu.incrementPC();
    }
};

// INC Rdest
// dest = dest + 1
class IncInstruction : public Instruction {
private:
    int dest;
public:
    IncInstruction(int d) { dest = d; }
    void execute(CPU &cpu) {
        int res = cpu.getRegister(dest).getValue() + 1;
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// DEC Rdest
// dest = dest - 1
class DecInstruction : public Instruction {
private:
    int dest;
public:
    DecInstruction(int d) { dest = d; }
    void execute(CPU &cpu) {
        int res = cpu.getRegister(dest).getValue() - 1;
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// =======================
// Member 3: Ammar
// =======================

// Custom Queue for instructions
class MyQueue {
private:
    Instruction* arr[128]; // fixed-size array to hold instructions
    int front, rear, count; // indices and count for queue management
public:
    MyQueue(): front(0), rear(0), count(0) {}
    bool isEmpty(){ return count==0; } // check if queue is empty
    bool isFull(){ return count==128; } // check if queue is full
    void enqueue(Instruction* instr){ // add instruction to queue
        if(!isFull()){ arr[rear]=instr; rear=(rear+1)%128; count++; }
    }
    Instruction* dequeue(){ // remove instruction from queue
        if(!isEmpty()){ Instruction* temp=arr[front]; front=(front+1)%128; count--; return temp; }
        return nullptr; // return null if empty
    }
};

// LOAD Rdest, [addr] → copy value from memory into a register
class LoadInstruction : public Instruction {
private: int dest, addr;
public:
    LoadInstruction(int d,int a):dest(d),addr(a){}
    void execute(CPU &cpu){
        int val=cpu.getMemory().read(addr); // read from memory
        cpu.getRegister(dest).setValue(val); // store in register
        cpu.updateFlags(val); // update flags
        cpu.incrementPC(); // move program counter
    }
};

// STORE [addr], Rsrc → copy value from register into memory
class StoreInstruction : public Instruction {
private: int src, addr;
public:
    StoreInstruction(int s,int a):src(s),addr(a){}
    void execute(CPU &cpu){
        int val=cpu.getRegister(src).getValue(); // get register value
        cpu.getMemory().write(addr,val); // write to memory
        cpu.updateFlags(val); // update flags
        cpu.incrementPC(); // move program counter
    }
};

// ROL Rdest → rotate bits left
class RolInstruction : public Instruction {
private: int dest;
public:
    RolInstruction(int d):dest(d){}
    void execute(CPU &cpu){
        unsigned char val=cpu.getRegister(dest).getValue();
        unsigned char res=(val<<1)|(val>>7); // rotate left
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// ROR Rdest → rotate bits right
class RorInstruction : public Instruction {
private: int dest;
public:
    RorInstruction(int d):dest(d){}
    void execute(CPU &cpu){
        unsigned char val=cpu.getRegister(dest).getValue();
        unsigned char res=(val>>1)|(val<<7); // rotate right
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// SHL Rdest → shift bits left (multiply by 2)
class ShlInstruction : public Instruction {
private: int dest;
public:
    ShlInstruction(int d):dest(d){}
    void execute(CPU &cpu){
        int res=cpu.getRegister(dest).getValue()<<1;
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// SHR Rdest → shift bits right (divide by 2)
class ShrInstruction : public Instruction {
private: int dest;
public:
    ShrInstruction(int d):dest(d){}
    void execute(CPU &cpu){
        int res=cpu.getRegister(dest).getValue()>>1;
        cpu.getRegister(dest).setValue(res);
        cpu.updateFlags(res);
        cpu.incrementPC();
    }
};

// INPUT Rdest → read value from user input into a register
class InputInstruction : public Instruction {
private: int dest;
public:
    InputInstruction(int d):dest(d){}
    void execute(CPU &cpu){
        int val; cout<<"Enter value: "; cin>>val; // ask user
        cpu.getRegister(dest).setValue(val); // store in register
        cpu.updateFlags(val);
        cpu.incrementPC();
    }
};

// DISPLAY Rsrc → print register value to console
class DisplayInstruction : public Instruction {
private: int src;
public:
    DisplayInstruction(int s):src(s){}
    void execute(CPU &cpu){
        cout<<"DISPLAY R"<<src<<" = "<<cpu.getRegister(src).getValue()<<endl;
        cpu.incrementPC();
    }
};

// Runner class → manages queue of instructions and executes them
class Runner {
private:
    MyQueue instrQueue; // queue of instructions
    CPU cpu;            // CPU instance
public:
    void loadProgram(){
        // Example program: enqueue instructions manually
        instrQueue.enqueue(new InputInstruction(0));       // INPUT R0
        instrQueue.enqueue(new StoreInstruction(0,10));    // STORE [10],R0
        instrQueue.enqueue(new LoadInstruction(1,10));     // LOAD R1,[10]
        instrQueue.enqueue(new ShlInstruction(1));         // SHL R1
        instrQueue.enqueue(new DisplayInstruction(1));     // DISPLAY R1
    }
    void run(){
        // Execute instructions until queue is empty
        while(!instrQueue.isEmpty()){
            Instruction* instr=instrQueue.dequeue();
            instr->execute(cpu);
            delete instr; // free memory
        }
        cpu.displayState(); // show final CPU state
    }
};

// =======================
// Main
// =======================
int main(){
    // ===Register Tests===
    Register R0;
    R0.setValue(50);
    cout<<"R0 value: "<<R0.getValue()<<endl;
    R0.setValue(200);
    cout<<"R0 max clamp: "<<R0.getValue()<<endl;
    R0.setValue(-200);
    cout<<"R0 min clamp: "<<R0.getValue()<<endl;
    R0.setValue(42);
    R0.display();

    // ===GeneralRegister Tests===
    GeneralRegister registers[8] = {0,1,2,3,4,5,6,7};
    registers[0].setValue(5);
    registers[1].setValue(10);
    registers[3].setValue(200);
    for(int i=0;i<8;i++){
        cout<<"R"<<i<<" = "<<registers[i].getValue()<<endl;
    }

    // ===FlagRegister Tests===
    FlagRegister flags;
    flags.displayFlags();
    flags.setOF(1);
    flags.displayFlags();
    flags.reset();
    flags.setZF(1);
    flags.displayFlags();

    // ===Memory Tests===
    Memory mem;
    mem.write(10,99);
    cout<<"mem[10] = "<<mem.read(10)<<endl;
    cout<<"mem[0]  = "<<mem.read(0)<<endl;

    // ===CPU Tests===
    CPU cpu;
    cpu.getRegister(0).setValue(77);
    cout<<"CPU R0 = "<<cpu.getRegister(0).getValue()<<endl;
    cpu.getMemory().write(5,33);
    cout<<"CPU mem[5] = "<<cpu.getMemory().read(5)<<endl;
    cpu.incrementPC();
    cout<<"CPU PC = "<<(int)cpu.getPC()<<endl;

    // ===displayState Test===
    CPU cpu2;
    cpu2.getRegister(1).setValue(17);
    cpu2.getRegister(3).setValue(68);
    cpu2.incrementPC();
    cpu2.incrementPC();
    cpu2.getMemory().write(20,68);
    cpu2.displayState();

    // ===Instruction Tests (Adeeb)===
    cout<<dec; //displayState uses hex so gotta switch back to normal numbers here
    cout<<"\n===Instruction Tests==="<<endl;
    CPU cpu3;
    cpu3.getRegister(1).setValue(10);

    // test MOV register-to-register mode
    Instruction* movReg = new MovInstruction(0,1,false); // MOV R0, R1
    movReg->execute(cpu3);
    cout<<"R0 after MOV R0,R1: "<<cpu3.getRegister(0).getValue()<<endl;

    // test MOV immediate mode
    Instruction* movImm = new MovInstruction(4, 99, 0); // MOV R4, 99  (mode 0 = immediate)
    movImm->execute(cpu3);
    cout<<"R4 after MOV R4,99: "<<cpu3.getRegister(4).getValue()<<endl;

    // test MOV register-indirect mode
    cpu3.getMemory().write(30, 55); // put 55 at memory address 30
    cpu3.getRegister(5).setValue(30); // R5 holds the address 30
    Instruction* movInd = new MovInstruction(6,5,true); // MOV R6, [R5]
    movInd->execute(cpu3);
    cout<<"R6 after MOV R6,[R5]: "<<cpu3.getRegister(6).getValue()<<endl;

    // test ADD
    Instruction* add = new AddInstruction(0,1); // ADD R0,R1
    add->execute(cpu3);
    cout<<"R0 after ADD: "<<cpu3.getRegister(0).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // test SUB
    Instruction* sub = new SubInstruction(0,1); // SUB R0,R1
    sub->execute(cpu3);
    cout<<"R0 after SUB: "<<cpu3.getRegister(0).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // test MUL
    cpu3.getRegister(0).setValue(5);
    cpu3.getRegister(1).setValue(6);
    Instruction* mul = new MulInstruction(0,1); // MUL R0,R1
    mul->execute(cpu3);
    cout<<"R0 after MUL: "<<cpu3.getRegister(0).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // test DIV normal case
    cpu3.getRegister(0).setValue(20);
    cpu3.getRegister(1).setValue(4);
    Instruction* div1 = new DivInstruction(0,1); // DIV R0,R1 -> 20/4
    div1->execute(cpu3);
    cout<<"R0 after DIV (20/4): "<<cpu3.getRegister(0).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // test DIV by zero (should not crash, dest stays same)
    cpu3.getRegister(0).setValue(20);
    cpu3.getRegister(1).setValue(0);
    Instruction* div2 = new DivInstruction(0,1);
    div2->execute(cpu3);
    cout<<"R0 after DIV by zero (unchanged): "<<cpu3.getRegister(0).getValue()<<endl;

    // test INC
    cpu3.getRegister(2).setValue(9);
    Instruction* inc = new IncInstruction(2); // INC R2
    inc->execute(cpu3);
    cout<<"R2 after INC: "<<cpu3.getRegister(2).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // test DEC
    cpu3.getRegister(2).setValue(0);
    Instruction* dec = new DecInstruction(2); // DEC R2
    dec->execute(cpu3);
    cout<<"R2 after DEC: "<<cpu3.getRegister(2).getValue()<<endl;
    cpu3.getFlags().displayFlags();

    // cleanup individual test pointers
    delete movReg;
    delete movImm;
    delete movInd;
    delete add;
    delete sub;
    delete mul;
    delete div1;
    delete div2;
    delete inc;
    delete dec;

    cout<<"\n===Polymorphism Demo (MyVector<Instruction*>)==="<<endl;
    CPU cpu4;

    MyVector<Instruction*> program; // our custom vector, not STL vector
    program.push_back(new MovInstruction(1, 5, 0)); // MOV R1, 5 (mode 0 = immediate)
    program.push_back(new AddInstruction(1, 1)); // ADD R1, R1 -> R1=R1+R1
    program.push_back(new MovInstruction(3, 1, false)); // MOV R3, R1
    program.push_back(new MulInstruction(3, 1)); // MUL R3, R1
    program.push_back(new IncInstruction(3)); // INC R3
    program.push_back(new DecInstruction(1)); // DEC R1

    for (int i = 0; i < program.size(); i++) {
        Instruction* instr = program.get(i); // base class pointer
        instr->execute(cpu4);                // virtual dispatch happens here
        cout<<"after instruction "<<i<<" -> R1="<<cpu4.getRegister(1).getValue()
            <<" R3="<<cpu4.getRegister(3).getValue()
            <<" PC="<<(int)cpu4.getPC()<<endl;
    }

    // cleanup the program vector
    for (int i = 0; i < program.size(); i++) {
        delete program.get(i);
    }

    // Runner Demo (Ammar)
    // Show how Runner executes a small program
    cout << "\n===Runner Demo (Ammar)===\n"; // print section header
    Runner runner;            // create Runner object
    runner.loadProgram();     // create Runner object
    runner.run();             // execute instructions and show CPU state

    return 0;
}
