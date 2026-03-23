#include <boost/asio.hpp>
#include <iostream>
#include <chrono>

class TickingTimer {
public:
    TickingTimer(boost::asio::io_context& io, int max_ticks)
        : timer_(io, boost::asio::chrono::seconds(1)), // Start first tick in 1s
          max_ticks_(max_ticks),
          count_(0) 
    {
        // Kickstart the loop
        start_wait();
    }

private:
    void start_wait() {
        // Capture 'this' by pointer to access member variables
        timer_.async_wait([this](const boost::system::error_code& ec) {
            handle_tick(ec);
        });
    }

    void handle_tick(const boost::system::error_code& ec) {
        // Rule: Always check for cancellation first!
        if (ec == boost::asio::error::operation_aborted) {
            std::cout << "Timer was cancelled." << std::endl;
            return;
        }

        if (ec) {
            std::cout << "Error: " << ec.message() << std::endl;
            return;
        }

        count_++;
        std::cout << "Tick " << count_ << "/" << max_ticks_ << std::endl;

        if (count_ >= max_ticks_) {
            std::cout << "Reached max ticks. Stopping." << std::endl;
            return; // Stop calling start_wait(), io.run() will naturally exit
        }

        // Advance expiry perfectly without drift
        timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
        
        // Loop again
        start_wait();
    }   

    boost::asio::steady_timer timer_;
    int max_ticks_;
    int count_;
};

int main() {
    boost::asio::io_context io;
    TickingTimer my_timer(io, 5); // Tick 5 times, once per second
    io.run();
    return 0;
}

