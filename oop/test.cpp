#include <iostream>
#include <sstream>
#include <chrono>
#include <string>

using namespace std;

string generate_mock_input(int count) {
    stringstream ss;
    for (int i = 0; i < count; ++i) {
        ss << "500 ";
    }
    return ss.str();
}

void run_standard_version(const string& data) {
    stringstream test_stream(data);
    int sum = 0;
    int temp;
    
    auto start = chrono::high_resolution_clock::now();
    while (test_stream >> temp) {
        if (temp > 0 && temp < 1001) sum += temp;
    }
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration = end - start;
    cout << "Standard Stream Time: " << duration.count() << " ms\n";
}

void run_ultra_fast_version(const string& data) {
    int sum = 0;
    
    auto start = chrono::high_resolution_clock::now();
    
    // Low-level pointer parsing (Skips stream conversion overhead entirely)
    const char* p = data.c_str();
    while (*p) {
        // Skip spaces manually
        while (*p == ' ') p++;
        if (!*p) break;
        
        // Inline integer extraction
        int val = 0;
        while (*p >= '0' && *p <= '9') {
            val = val * 10 + (*p - '0');
            p++;
        }
        
        if (val > 0 && val < 1001) sum += val;
    }
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << "Ultra-Fast Raw Pointer Time: " << duration.count() << " ms\n";
}

int main() {
    cout << "Generating 1,000,000 numbers for the test...\n";
    string input_data = generate_mock_input(1000000);
    
    cout << "Starting tests...\n\n";
    run_standard_version(input_data);
    run_ultra_fast_version(input_data);
    
    return 0;
}
