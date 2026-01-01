
#include <iostream>
#include <chrono>
using namespace std;

int main() {
    int sum = 0;
    auto start = chrono::high_resolution_clock::now();
    
    int temp;
    while (cin >> temp) {
        if (temp > 0 && temp < 1001) {
            sum += temp;
        }
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << duration.count() << " ms" << endl;
    return 0;
}
