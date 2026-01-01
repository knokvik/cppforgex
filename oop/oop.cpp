#include <iostream>
using namespace std;

struct record {
    int rid = 101;
    void getOOP();
    void exit();
};

class node {
    public:
    inline static int usercame = 0;

    node() {
        cout << "node createad";
    };

    int rid = 101;
    void getOOP() {
        cout << "getOOP Func Called";
    }

    void exit() {
        cout << "exittnig oop";
    };

    friend void friendfn( node& obj );

    ~node() {
        cout << "object destroyed";
    };
};

//friend function 
void friendfn( node& n1 ) {
    cout << endl << n1.rid;
}

void oophere() {
    node n1;
    node::usercame++;
    cout << endl << "user came here : " << node::usercame << endl;
    cout << n1.rid;
    n1.getOOP();
}

// ==========================================
// CRITICAL CONCEPT: THE DIAMOND PROBLEM
// ==========================================
// Occurs in multiple inheritance when two classes inherit from the same base class,
// and another class inherits from both of those two classes.
// This causes ambiguity and duplicates the base class data.

class BaseDevice {
public:
    int power_level = 100;
    BaseDevice() { cout << "\nBaseDevice created\n"; }
};

// ❌ Without 'virtual', Scanner and Printer each get their own copy of BaseDevice!
// To fix this, use 'virtual public BaseDevice'
class Scanner : virtual public BaseDevice {
public:
    Scanner() { cout << "Scanner created\n"; }
};

class Printer : virtual public BaseDevice {
public:
    Printer() { cout << "Printer created\n"; }
};

// Copier inherits from both Scanner and Printer.
class Copier : public Scanner, public Printer {
public:
    Copier() { cout << "Copier created\n"; }
    
    void showPower() {
        // If virtual inheritance wasn't used, this line would throw a compiler error:
        // "request for member 'power_level' is ambiguous"
        // because Copier would have TWO power_levels (one from Scanner, one from Printer).
        cout << "Power level: " << power_level << "\n";
    }
};

void diamond_problem_demo() {
    cout << "\n--- Diamond Problem Demo ---\n";
    Copier myCopier;
    myCopier.showPower();
}