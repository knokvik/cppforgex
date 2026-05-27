#include <iostream>
#include <thread>
#include <vector>

void worker(int id) {
    std::cout << "Thread " << id << " executing\n";
}

int main() {
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 4; ++i) {
        threads.emplace_back(worker, i);
    }
    
    for(auto& t : threads) {
        t.join();
    }
    return 0;
}
// Enforced thread safety
