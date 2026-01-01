#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// ==========================================
// 1. THE BAD APPROACH: Causes a Crash/UB
// ==========================================
class BrokenNetworkWorker {
public:
    std::string mock_html_payload = "<html>Target Content</html>";

    void start_async_download() {
        std::cout << "[Broken] Initiating asynchronous background download...\n";
        
        // DANGER LAMBDA: Capturing 'this' via a raw pointer!
        std::thread background_card([this]() {
            // Simulate waiting 200 milliseconds for network packets
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            // CRASH HERE: If the main thread deleted the object, 
            // 'this' is pointing to trash/garbage memory!
            std::cout << "[Broken] Background thread reading data: " 
                      << this->mock_html_payload << "\n";
        });
        background_card.detach(); // Let thread run independently in background
    }
    
    ~BrokenNetworkWorker() {
        std::cout << "[Broken] Destructor executed! Object memory freed.\n";
    }
};

// ==========================================
// 2. THE PRODUCTION APPROACH: Guaranteed Safe
// ==========================================
class FixedNetworkWorker : public std::enable_shared_from_this<FixedNetworkWorker> {
public:
    std::string mock_html_payload = "<html>Target Content</html>";

    void start_async_download() {
        std::cout << "[Fixed] Initiating asynchronous background download...\n";
        
        // THE FIX: Grab a strong shared pointer tied to the ORIGINAL control block!
        auto self = shared_from_this(); 
        
        // SAFE LAMBDA: We capture 'self' by value!
        // This explicitly increments the reference count of this object instance.
        std::thread background_card([self, this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            // GUARANTEED SAFE: The object cannot be destroyed while 'self' exists.
            std::cout << "[Fixed] Background thread reading data safely: " 
                      << this->mock_html_payload << "\n";
        });
        background_card.detach();
    }
    
    ~FixedNetworkWorker() {
        std::cout << "[Fixed] Destructor executed! Object memory freed cleanly.\n";
    }
};

// ==========================================
// EXECUTION SCOPE TRACE
// ==========================================
void trigger_broken_behavior() {
    std::cout << "\n--- starting broken lifecycle demo ---\n";
    {
        auto session = std::make_shared<BrokenNetworkWorker>();
        session->start_async_download();
    } // <-- 'session' goes out of scope here. Object is INSTANTLY deleted!
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void trigger_fixed_behavior() {
    std::cout << "\n--- starting fixed lifecycle demo ---\n";
    {
        auto session = std::make_shared<FixedNetworkWorker>();
        session->start_async_download();
    } // <-- 'session' goes out of scope here. 
      // But destructor is NOT called because background lambda holds 'self'!
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

int main() {
    trigger_fixed_behavior();
    trigger_broken_behavior();
    return 0;
}
