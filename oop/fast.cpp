
#include <iostream>
#include <chrono>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int sum = 0;
    auto start = chrono::high_resolution_clock::now();
    
    while (true) {
        if (int temp; cin >> temp) {
            if (temp > 0 && temp < 1001) sum += temp;
        } else {
            break;
        }
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << duration.count() << " ms\n";
    return 0;
}
