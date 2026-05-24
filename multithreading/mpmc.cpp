#include <atomic>
#include <vector>
#include <string>
#include <optional>
#include <thread>
#include <iostream>

template <typename T>
class LockFreeMPMC {
private:
    struct Node {
        T data;
        // Tracks the lifecycle stage of this specific slot
        std::atomic<size_t> sequence;
    };

    std::vector<Node> buffer_;
    const size_t buffer_mask_;
    
    // Align indices to distinct CPU Cache Lines to prevent False Sharing
    alignas(64) std::atomic<size_t> enqueue_pos_{0};
    alignas(64) std::atomic<size_t> dequeue_pos_{0};

public:
    // Capacity must be a power of 2 for fast bitwise modulo masking
    LockFreeMPMC(size_t capacity) 
        : buffer_(capacity), buffer_mask_(capacity - 1) {
        for (size_t i = 0; i < capacity; ++i) {
            buffer_[i].sequence.store(i, std::memory_order_relaxed);
        }
    }

    // Multiple Producer Threads call this concurrently
    bool push(T item) {
        Node* node;
        size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        
        while (true) {
            node = &buffer_[pos & buffer_mask_];
            size_t seq = node->sequence.load(std::memory_order_acquire);
            intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);

            if (diff == 0) {
                // Slot is empty and ready for this specific position loop turn.
                // Try to claim this slot before any other producer thread does.
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break; 
                }
            } else if (diff < 0) {
                // Queue is completely full, backpressure triggered
                return false;
            } else {
                // Another thread beat us to it, re-read the fresh position state
                pos = enqueue_pos_.load(std::memory_order_relaxed);
            }
        }

        // Write the data payload safely into our claimed slot
        node->data = std::move(item);
        
        // Release Step: Signal to consumers that this slot is completely written
        node->sequence.store(pos + 1, std::memory_order_release);
        return true;
    }

    // Multiple Consumer Threads call this concurrently
    std::optional<T> pop() {
        Node* node;
        size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
        
        while (true) {
            node = &buffer_[pos & buffer_mask_];
            size_t seq = node->sequence.load(std::memory_order_acquire);
            intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);

            if (diff == 0) {
                // Slot has data and is ready to be parsed.
                // Try to claim this slot before any other consumer thread does.
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (diff < 0) {
                // Queue is completely empty
                return std::nullopt;
            } else {
                // Another thread beat us to it, re-read the fresh position state
                pos = dequeue_pos_.load(std::memory_order_relaxed);
            }
        }

        // Extract the data payload safely from our claimed slot
        T result = std::move(node->data);
        
        // Release Step: Signal to producers that this slot is now empty and recycled
        node->sequence.store(pos + buffer_mask_ + 1, std::memory_order_release);
        return result;
    }
};
// Enforced thread safety
// Suppressed unused warnings
