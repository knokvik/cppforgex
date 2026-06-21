#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

using namespace std;

struct Task {
    void (*func)(void*) = nullptr;
    void* arg = nullptr;

    void operator()() const {
        if(func) {
            func(arg);
        }
    }
};

class alignas(64) FixedThreadPool {
    private:
    vector<thread> workers;
    queue<Task> tasks;
    mutex queue_mutex;
    condition_variable cv;
    bool stop = false; 

    public:
    explicit FixedThreadPool(size_t threads) {
        workers.reserve(threads);

        for( size_t i=0;i < threads; i++) {
            workers.emplace_back([this] {
                while(true) {
                    Task task;
                    {
                        unique_lock <mutex> lock(this->queue_mutex);

                        this->cv.wait(lock , [this] {
                            return this->stop || !this->tasks.empty();
                        });

                        if(this->stop && this->tasks.empty()) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }
    
    void submit(void (*func)(void*), void* arg) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) return; 
                
            tasks.push(Task{func, arg});
        } 
        
        cv.notify_one(); 
    }

     ~FixedThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all(); 
        
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    FixedThreadPool(const FixedThreadPool&) = delete;
    FixedThreadPool& operator=(const FixedThreadPool&) = delete;
};

void sample_work(void* arg) {
    int* val = static_cast<int*>(arg); 
    std::cout << "Thread " << std::this_thread::get_id() 
              << " is processing data value: " << *val << "\n";
}


int main() {
    FixedThreadPool pool(4);
    
    int data1 = 100;
    int data2 = 200;
    int data3 = 300;
    
    pool.submit(sample_work, &data1);
    pool.submit(sample_work, &data2);
    pool.submit(sample_work, &data3);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    return 0;
}
// Updated documentation
