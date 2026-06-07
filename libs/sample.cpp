#include <iostream>

using namespace std;

class sample {
    public:
    void operator()() const {
        cout << "hello from func";
    }
};

int main() {
    sample s;
    s();
    return 0;
}// Suppressed unused warnings
