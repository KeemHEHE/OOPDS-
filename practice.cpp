#include <iostream>
using namespace std;

class Box {
    private:
        int number;

    public:
        // constructor - runs automatically when box is created
        // sets number to 0 by default
        Box() {
            number = 0;
        }
         void setNumber (int n) {
            // only allow numbers between -128 and 127
            if (n>127) {
                cout << "TOO BIG! Max is 127" <<endl;
                number = 127; // store max value
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

    //read before putting anything in
    cout << "Default value:" << myBox.getNumber() << endl;

    //now put something in
    myBox.setNumber(42);
    cout << "After setting:" << myBox.getNumber() << endl;

    //test freshBox
    Box freshBox; //brand new box didnt store old value auto set = 0
    cout << "Default value:" << freshBox.getNumber() << endl;

    freshBox.setNumber(42);
    cout << "After setting:" << freshBox.getNumber() << endl;




    return 0;
}