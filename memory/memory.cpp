#include <iostream>
#include <memory>

using namespace std;

// --- RULE A (The General Rule) ---
template <typename T>
struct MemoryInspector {
    static void print() { std::cout << "Normal variable"; }
};

// --- RULE B (The Exception Rule for Pointers) ---
template <typename T>
struct MemoryInspector<T*> {  
    static void print() { std::cout << "Pointer variable"; }
};

int main() {
    // You call the structs inside main like this:
    MemoryInspector<int>::print();      // Prints: Normal variable
    std::cout << std::endl;
    MemoryInspector<int*>::print();     // Prints: Pointer variable
    
    return 0;
}
