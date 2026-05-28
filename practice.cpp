#include <iostream>
using namespace std;

class Register {
    private:
        signed char value; //changed from int to signed char

    public:
        // constructor - runs automatically when box is created
        // sets number to 0 by default
        Register() {
            value = 0;
        }
        virtual ~Register() {}
         void setValue (int n) { //renamed from setNumber
            // only allow numbers between -128 and 127
            if (n>127) {
                value = 127; // store max value
            } else if (n<-128) {
                value = -128; // store min value
            } 
            else {
                value = n; //normal, just store it
            }
        }
        int getValue() {
            return value;
        }

        // print the value nicely
         void display () {
             cout << "Register value: " << (int)value << endl;
            }
};

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




        class FlagRegister {
        private:
            int OF; //overflow flag
            int UF; //underflow flag
            int CF; //carry flag
            int ZF; //zero flag
            
            
        public:
            // constructor - all flags start at 0 (OFF)
            FlagRegister() {
                OF = 0;
                UF = 0;
                CF = 0;
                ZF = 0;
            }

            //turn OF on or off
            void setOF(int v) {
                OF = v;
                }

            //turn UF on or off
            void setUF(int v) {
                UF = v;
                }

            //turn CF on or off
            void setCF(int v) {
                CF = v;
                }

            //turn ZF on or off
            void setZF(int v) {
                ZF = v;
                }

            // read ZF
            int getZF() {
                return ZF;
            }

            // read CF
            int getCF() {
                return CF;
            }

            // read UF
            int getUF() {
                return UF; 
            }

            // read OF
            int getOF() {
                return OF;
            }

            void displayFlags() {
            cout << "OF=" << OF << " UF=" << UF << " CF=" << CF << " ZF=" << ZF << endl;
            }

            void reset (){
            OF = 0;
            UF = 0;
            CF = 0;
            ZF = 0;
            }
        
};

class Memory {
    private:
           signed char data[64]; // 64 boxes, each holds -128 to 127

    public:
           Memory() {
               // fill all 64 slots with 0 at the start 
               for (int i = 0; i< 64; i++) {
                   data[i] = 0;
               }
           }

           void write(int address, int value) {
               if (address >= 0 && address <= 63) {
                    data[address]=value;
               }
     
           }

           int read (int address) {
            if (address >= 0 && address <= 63) {
                return data[address];
            }
            else {
                return 0;
            }
           
           }


            

            
};
    



        

        


int main() {
   
    //1.Basic Register usage
    cout << "===Register Tests===" << endl;

    //test Register
    Register R0;
    R0.setValue(50);
    cout << "R0 value:" << R0.getValue() << endl;

    //test too big number
    R0.setValue(200);
    cout << "R0  max value:" << R0.getValue() << endl;
   

    //test too small number
    R0.setValue(-200);
    cout << "R0 min value:" << R0.getValue() << endl;

    //test fresh register starts at 0
    Register R1;
    cout << "R1 default:" << R1.getValue() << endl;

    //test display function
    R0.setValue(42);
    R0.display();


    //2.Test array of 8 registers
    cout << "\n===Array of Registers Tests===" << endl;

    // ADD 8 REGISTER 
    Register registers [8];

    registers[0].setValue(5);
    registers[1].setValue(10);
    registers[2].setValue(15);
    registers[3].setValue(200);


    cout << "\nAll 8 Registers:" << endl;
    for (int i = 0; i < 8; i++) {
        cout << "Registers" << i << " = "
        << registers[i].getValue() << endl;
    }

    //3. FlagRegister
    cout << "\n===FlagRegister Tests===" << endl;


    FlagRegister flags;
    cout << "Initial flags:";
    flags.displayFlags();

    Register R2;
    R2.setValue(200);  //trigger overflow
    flags.setOF(1);
    cout << "After overflow:";
    flags.displayFlags();

    flags.reset();
    R2.setValue(-200); //trigger underflow
    flags.setUF(1);
    cout << "After underflow:";
    flags.displayFlags();

    flags.reset();
    R2.setValue(0); //trigger zero
    flags.setZF(1);
    cout << "After zero:";
    flags.displayFlags();

    flags.reset();
    flags.setCF(1);
    cout << "After carry:";
    flags.displayFlags();

    cout << "\nReading individual flags:" << endl;
    cout << "getOF=" << flags.getOF() << endl;
    cout << "getUF=" << flags.getUF() << endl;
    cout << "getCF=" << flags.getCF() << endl;
    cout << "getZF=" << flags.getZF() << endl;
    


    return 0;

  
}