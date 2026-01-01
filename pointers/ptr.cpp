#include <iostream>
using std::cout;
using std::endl;

void ptrhere() {

    int        val  = 10;
    int        target = 20;

    // 1. Pointer to Variable Data (Standard)
    int*       p1   = &val; 
    *p1 = 15;                     // Allowed: Data can change
    p1  = &target;                // Allowed: Address can change

    // 2. Pointer to Constant Data (Data is locked, Address is free)
    const int*       p2   = &val; 
    // *p2 = 15;                  // ❌ Compiler Error: Data is read-only!
    p2  = &target;                // Allowed: Pointer can point elsewhere

    // 3. Constant Pointer to Variable Data (Address is locked, Data is free)
    int* const p3   = &val; 
    *p3 = 15;                     // Allowed: Data can change
    // p3  = &target;             // ❌ Compiler Error: Address is locked!

    // 4. Constant Pointer to Constant Data (Everything is locked)
    const int* const p4   = &val; 
    // *p4 = 15;                  // ❌ Compiler Error: Data locked
    // p4  = &target;             // ❌ Compiler Error: Address locked


    int arr[5] = { 1 , 2 , 3 , 4, 5};
    int* p = arr;
    cout << "Pointer value :" << *p << endl;
    p+=1;
    cout << "Pointer value: " << *p;

    cout << arr[2];     // Evaluates to *(arr + 2) -> Prints 30
    cout << *(arr + 2); // Explicit pointer arithmetic -> Prints 30
    cout << 2[arr];     // Valid C++! Evaluates to *(2 + arr) -> Prints 30

}

void memoryAnamolies() {
    // Cast nullptr to a generic data pointer (void*) to resolve ambiguity
    std::cout << NULL << std::endl << (void*)nullptr;

    int price = 500;
    int* ptr = &price;      // Stores address of 'price'
    int** dbl_ptr = &ptr;   // Stores address of 'ptr'

    std::cout << *dbl_ptr;  // Dereference 1: Prints the address stored in 'ptr'
    std::cout << **dbl_ptr; // Dereference 2: Jumps again, prints 500

}

void heapManagement() {
    // 1. Single Object Management
    int* p_val = new int(100); // Allocates 4 bytes on Heap
    delete p_val;              // Deallocates memory
    p_val = nullptr;           // Clears the address

    // 2. Dynamic Array Management
    int* p_arr = new int[50];  // Allocates contiguous array on Heap
    delete[] p_arr;            // ❌ Crucial: You MUST use delete[] with brackets!
    p_arr = nullptr;
}
    
void trigger_leak() {
    int* leak_ptr = new int[100]; // Allocated but never deleted
}

// ==========================================
// CRITICAL CONCEPT: DANGLING POINTERS
// ==========================================
// A dangling pointer points to memory that has already been freed or is out of scope.
// Dereferencing it causes undefined behavior (often a crash).

int* create_dangling_pointer() {
    int local_var = 42;
    // ❌ Returning address of a local variable! 
    // When function exits, 'local_var' is destroyed, but we return its address.
    return &local_var; 
}

void dangling_demo() {
    int* bad_ptr = create_dangling_pointer();
    // cout << *bad_ptr; // 💥 CRASH OR GARBAGE DATA!
    
    // Another way to create a dangling pointer:
    int* heap_ptr = new int(10);
    delete heap_ptr; // Memory is freed.
    // heap_ptr is now dangling! 
    // Fix: ALWAYS set to nullptr after delete.
    heap_ptr = nullptr;
}

// ==========================================
// CRITICAL CONCEPT: SMART POINTERS (Modern C++)
// ==========================================
// To avoid memory leaks and dangling pointers, use C++11 smart pointers from <memory>.
#include <memory>

void smart_pointer_demo() {
    // 1. std::unique_ptr (Exclusive Ownership)
    // Memory is automatically freed when the unique_ptr goes out of scope.
    std::unique_ptr<int> u_ptr = std::make_unique<int>(100);
    // std::unique_ptr<int> u_ptr2 = u_ptr; // ❌ ERROR: Cannot copy unique_ptr!
    std::unique_ptr<int> u_ptr_moved = std::move(u_ptr); // ✅ OK: Transfer ownership
    
    // 2. std::shared_ptr (Shared Ownership)
    // Uses reference counting. Memory is freed when the LAST shared_ptr is destroyed.
    std::shared_ptr<int> s_ptr1 = std::make_shared<int>(200);
    std::shared_ptr<int> s_ptr2 = s_ptr1; // ✅ OK: Ref count becomes 2.
}