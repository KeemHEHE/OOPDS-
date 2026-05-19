#include <iostream>
using namespace std;

class Box {
    private:
        int number;

    public:
         void setNumber (int n) {
            // only allow numbers between -128 and 127
            if (n>127) {
                cout << "TOO BIG! Max is 127" <<endl;
                number = 127; // store max value
            }
            } else if (n<-128) {
                cout << "TOO SMALL! Min is -128" <<endl;
                number = -128; // store min value
            } 
            else {
                number = n; //normal, just store it
            }
        }
        int getNumber() {
            return number;
        }

};

int main() {
    Box myBox;

    //test normal number
    myBox.setNumber(50);
    cout << "Value:" << myBox.getNumber() << endl;

    //test too big number
    myBox.setNumber(200);
    cout << "Value:" << myBox.getNumber() << endl;

    //test too small number
    myBox.setNumber(-200);
    cout << "Value:" << myBox.getNumber() << endl;

    return 0;
}