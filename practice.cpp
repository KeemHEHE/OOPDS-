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
         void setValue (int n) { //renamed from setNumber
            // only allow numbers between -128 and 127
            if (n>127) {
                cout << "OVERFLOW! Value too big" <<endl;
                value = 127; // store max value
            } else if (n<-128) {
                cout << "UNDERFLOW! Value too small" <<endl;
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
             cout << "Register value: " << value << endl;
            }
};
        


int main() {
   

    //test normal number
    Register R0;
    R0.setValue(50);
    cout << "R0 value:" << R0.getValue() << endl;

    //test too big number
    R0.setValue(200);
    cout << "R0 value:" << R0.getValue() << endl;
   

    //test too small number
    R0.setValue(-200);
    cout << "R0 value:" << R0.getValue() << endl;

    //test fresh register starts at 0
    Register R1;
    cout << "R1 default:" << R1.getValue() << endl;

    //test display function
    R0.setValue(42);
    R0.display();


    return 0;
}