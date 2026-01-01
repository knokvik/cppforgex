#include <iostream>
#include <chrono>
#include <string.h>
#include "str.h"
#include "ptr.h"
#include "thread.h"
#include "fstream"
#include "arr.h"
#include "oop.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

void vectorize();
std::array<std::array<int,3>,3> arrhere();
void arrdecaycheck(std::array<std::array<int,3>,3>);
void strhere();

void ptrhere();
void memoryAnamolies();
void heapManagement();
void trigger_leak();

void oophere();

void fileHere();
void logUrl();

void threadOp();
void runParallel();

void testLoop() {
    
    auto start = std::chrono::steady_clock::now();
    int i=0;
    if(i==0) [[likely]] {
        for(i=0;i<=1000000;i++) {
            cout << i << endl;
        }
    } else [[unlikely]]{
        for(i=0;i<=1000000;i++) {
            cout << i << endl;
        }
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;    
    cout << "Time Taken : " << duration.count() << "s";
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // string name;
    // cout << "hello this is niraj here:";
    // cin >> name;
    // cout << "Name : " << name << endl;
    // int zhatu;
    // char a = 10;
    // char *p = &a;
    // cout << sizeof(p) << " bytes" << std::endl;
    // cout << "Address of pointer p: " << &p << std::endl;
    // cout << "Address inside p (a): " << (void*)p << std::endl; // Cast to void*
    // testLoop();
    // auto arr = arrhere();
    // arrdecaycheck(arr);

    // strhere();
    // vectorize();
    // ptrhere();

    // oophere();
    // fileHere();
    // logUrl();
    // threadOp();
    runParallel();
    // memoryAnamolies();
    // trigger_leak();
    // std::cout << "Program running. Press ENTER to close and run leak check...\n";
    // std::cin.get(); // 🚀 Temporarily holds the process alive in RAM
}

void process_url(std::string url, bool retry_on_fail) {
    // You wrote code to process the url...
    // But you completely forgot to implement the retry logic!
    std::cout << "Processing: " << url << "\n";
}
