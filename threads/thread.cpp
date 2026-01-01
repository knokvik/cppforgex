#include <iostream>
#include <thread>
#include <string>

void download_worker(std::string url, int worker_id) {
    std::cout << "Worker " << worker_id << " started downloading: " << url << "\n";
    // Simulate web download latency
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    std::cout << "Worker " << worker_id << " finished!\n";
}

void threadOp() {
    std::string target_url = "https://example.com";

    // 🚀 Spawn a background worker thread and pass parameters to it
    std::thread t1(download_worker, target_url, 1);
    std::thread t2(download_worker, target_url, 2);

    // CRITICAL REQUIREMENT: You must tell the main thread to wait for them!
    t1.join(); 
    t2.join(); 

    std::cout << "All downloads complete. Main thread exiting.\n";
}

//join() and detach()

#include <mutex>

int total_pages_crawled = 0; 
std::mutex counter_mutex; // The lock protection hub

void secure_worker() {
    for(int i = 0; i < 10000; ++i) {
        // 🛡️ Lock scope entry: Automatically locks the mutex on construction
        std::lock_guard<std::mutex> lock(counter_mutex); 
        
        total_pages_crawled++; 
        // 🔄 Automatically unlocks here as 'lock' goes out of scope!
    }
}

void runParallel() {
        std::thread t1(secure_worker);
        std::thread t2(secure_worker);
        t1.join();
        t2.join();
        std::cout << "Total Pages crawled : " << total_pages_crawled << std::endl;
}

// ==========================================
// CRITICAL CONCEPT: DEADLOCKS
// ==========================================
// A deadlock occurs when two or more threads are blocked forever, 
// each waiting on the other to release a lock.

std::mutex resource_A;
std::mutex resource_B;

void thread1_task() {
    std::lock_guard<std::mutex> lock1(resource_A);
    std::cout << "Thread 1 acquired Resource A\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate work

    // ❌ DANGER: Thread 1 needs B, but Thread 2 might hold it!
    std::lock_guard<std::mutex> lock2(resource_B);
    std::cout << "Thread 1 acquired Resource B\n";
}

void thread2_task() {
    std::lock_guard<std::mutex> lock1(resource_B);
    std::cout << "Thread 2 acquired Resource B\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate work

    // ❌ DANGER: Thread 2 needs A, but Thread 1 holds it! (CIRCULAR WAIT -> DEADLOCK)
    std::lock_guard<std::mutex> lock2(resource_A);
    std::cout << "Thread 2 acquired Resource A\n";
}

void trigger_deadlock() {
    std::cout << "\n--- Deadlock Demo (WARNING: This will freeze!) ---\n";
    std::thread t1(thread1_task);
    std::thread t2(thread2_task);
    t1.join();
    t2.join();
}

// 🛡️ SOLUTION TO DEADLOCK:
// 1. Always acquire locks in the SAME ORDER across all threads (e.g., A then B).
// 2. Or, use std::scoped_lock (C++17) to lock multiple mutexes simultaneously and safely:
// std::scoped_lock lock(resource_A, resource_B);
