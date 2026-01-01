#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <thread>
#include <vector>

class IngestionQueue {
private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool finished_ = false;

public:
    void push(std::string content) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(content));
        }
        cv_.notify_one(); 
    }

    bool pop(std::string& out_content) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        cv_.wait(lock, [this]() { 
            return !queue_.empty() || finished_; 
        });

        if (queue_.empty() && finished_) {
            return false;
        }

        out_content = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            finished_ = true;
        }
        cv_.notify_all();
    }
}; 

void consumer_worker(int id, IngestionQueue& queue) {
    std::string task;
    while (queue.pop(task)) {
        std::cout << "Consumer " << id << " processed task: " << task << "\n";
    }
}

int main() {
    IngestionQueue ingestion_pipeline;

    // We are strictly using std::jthread here!
    std::vector<std::jthread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back(consumer_worker, i, std::ref(ingestion_pipeline));
    }
   
    ingestion_pipeline.push("<html>Data A</html>");
    ingestion_pipeline.push("<html>Data B (Large payload)</html>");
    ingestion_pipeline.push("<html>Data C</html>");
  
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Once we request shutdown, the jthreads will finish their current loop 
    // and automatically join when main() ends. No manual loops required.
    ingestion_pipeline.shutdown();

    std::cout << "[System] Main thread exiting. jthreads destructing and joining...\n";
    return 0;
}