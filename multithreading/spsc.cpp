#include <atomic>
#include <string>
#include <vector>
#include <optional>

template <typename T, size_t Capacity>
class LockFreeSPSC {
private:
    // Ensure data storage size allocation accounts for a buffer slot layout
    std::vector<T> buffer_;
    
    // Align atomic indices to distinct CPU Cache Lines (typically 64 bytes)
    // This prevents 'False Sharing' where cores fight over adjacent variables.
    alignas(64) std::atomic<size_t> head_{0};
    alignas(64) std::atomic<size_t> tail_{0};

public:
    LockFreeSPSC() : buffer_(Capacity) {}

    // Executed purely by the Producer Thread
    bool push(T item) {
        const size_t current_head = head_.load(std::memory_order_relaxed);
        
        // Acquire tail index to verify remaining capacity bounds
        const size_t current_tail = tail_.load(std::memory_order_acquire);

        // Check if ring buffer is completely full
        if ((current_head + 1) % Capacity == current_tail) {
            return false; // Queue is full, drop task or backpressure
        }

        // Place data into the buffer slot safely
        buffer_[current_head] = std::move(item);

        // Release Store: Publishes new item to consumer, 
        // ensuring memory write finishes before index increments.
        head_.store((current_head + 1) % Capacity, std::memory_order_release);
        return true;
    }

    // Executed purely by the Consumer Thread
    std::optional<T> pop() {
        const size_t current_tail = tail_.load(std::memory_order_relaxed);
        
        // Acquire head index to verify read task availability status
        const size_t current_head = head_.load(std::memory_order_acquire);

        // Check if ring buffer is completely empty
        if (current_tail == current_head) {
            return std::nullopt; // Queue is empty, yield thread execution
        }

        // Extract the target payload item
        T item = std::move(buffer_[current_tail]);

        // Release Store: Signals to producer that this slot is now free
        tail_.store((current_tail + 1) % Capacity, std::memory_order_release);
        return item;
    }
};