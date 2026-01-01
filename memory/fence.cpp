#include <atomic>
#include <string>
#include <thread>
#include <iostream>
#include <chrono> // Required for time delays

std::string g_sharedPayload;       
std::atomic<bool> g_readyFlag{false}; 

void producerThread() {
    // 1. Force this thread to freeze and sleep for 1 entire second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_sharedPayload = "Secret Engine Data Pack"; 
}

void consumerThread() {
    // 2. The consumer wakes up immediately and instantly tries to read the text
    std::cout << "Received payload: " << g_sharedPayload << "\n";
}

int main() {
    std::cout << "[System] Launching concurrent threads...\n";

    std::thread producer(producerThread);
    std::thread consumer(consumerThread);

    producer.join();
    consumer.join();

    std::cout << "[System] Execution completed.\n";
    return 0;
}
