#include<iostream>
#include <array>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

std::array<std::array<int,3>,3> arrhere() {
    //    int arr[5] = { 1 , 2 , 3, 4, 5};
       std::array<std::array<int, 3>, 3> arr = {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }};
       for( int i =0;i<=sizeof(arr) / sizeof(arr[0])-1;i++ ) {
        cout << "["<<i<<"] -> ";
           for(int j = 0;j<=sizeof(arr) / sizeof(arr[0])-1;j++) {
                 cout << &arr[j] << "\t";
           }
           cout << std::endl;
       }
       return arr;
}

//arr decay
void arrdecaycheck( std::array<std::array<int,3>,3> arr) {
    cout << "\nArray Decay Address : " << &arr[0][0] << std::endl;
}

void vectorize() {
    vector<int> vr;
    cout << endl;
    vr.push_back(1);
    cout << vr.capacity() << endl;
    vr.push_back(2);
    cout << vr.capacity() << endl;
    vr.push_back(3);
    cout << vr.capacity() << endl;

    // 🚀 Method A: Optimized push_back
    std::vector<int> vec1;
    vec1.reserve(1000); // Allocates capacity for 1000 integers. size = 0.
    for(int i = 0; i < 1000; ++i) vec1.push_back(i); // Hyper-fast O(1), zero reallocations.
    cout << "Reserve: " << vec1.size() << "\t" << vec1.capacity() << endl;
    for( int i=0;i<1000;i++) { cout << vec1.at(i);if(i%10==0) cout << endl;};

    // 🚀 Method B: Direct Element Manipulation
    std::vector<int> vec2;
    vec2.resize(1000); // Allocates AND initialises 1000 integers (all set to 0). size = 1000. // Direct lookup insertion.
    cout << "Resize: " << vec2.size() << "\t" << vec2.capacity() << endl;
    for(int i=0;i<1000;i++) { cout << vec2.at(i);if(i%10==0) cout << endl;};

}    

// ==========================================
// CRITICAL CONCEPT: ITERATOR INVALIDATION
// ==========================================
// When a vector grows beyond its capacity, it allocates a new larger block of memory,
// copies old elements, and deletes the old block. Any pointers or iterators pointing 
// to the old block become INVALID (dangling).

void iterator_invalidation_demo() {
    std::vector<int> numbers = {1, 2, 3};
    auto it = numbers.begin(); // Points to '1'
    
    std::cout << "Iterator points to: " << *it << "\n"; // Prints 1
    
    // ❌ DANGER: push_back might trigger a reallocation if capacity is exceeded!
    numbers.push_back(4); 
    numbers.push_back(5); 
    numbers.push_back(6); 
    
    // If reallocation happened, 'it' is now a dangling pointer!
    // std::cout << "Iterator points to: " << *it << "\n"; // 💥 UNDEFINED BEHAVIOR!
    
    // Fix: Re-assign the iterator after operations that modify capacity.
    it = numbers.begin(); 
    std::cout << "Fixed Iterator points to: " << *it << "\n";
}

// ==========================================
// CRITICAL CONCEPT: OUT OF BOUNDS ACCESS
// ==========================================
void out_of_bounds_demo() {
    std::vector<int> vec = {10, 20, 30};
    // cout << vec[5]; // ❌ UNDEFINED BEHAVIOR (Doesn't check bounds, might crash or print garbage)
    
    try {
        // ✅ ALWAYS USE .at() if you are unsure of the index! It throws an exception instead of silent corruption.
        cout << vec.at(5); 
    } catch(const std::out_of_range& e) {
        std::cerr << "Out of bounds caught: " << e.what() << '\n';
    }
}