#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// 1. THE OBJECT TYPE
// This represents a heavy object that is expensive to initialize.
struct NetworkSocket {
    int socket_id;
    bool is_active;
    char buffer[1024]; // Internal data storage

    // Constructor: This is the heavy "Baking" phase
    void construct(int id) {
        socket_id = id;
        is_active = false;
        // Simulate a heavy operation (like allocating OS resources or hands-shakes)
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    // A method to reset the state without destroying the structural memory
    void mark_dormant() {
        is_active = false;
    }

    void mark_active() {
        is_active = true;
    }
};

// 2. THE SLAB ALLOCATOR
class SocketSlabAllocator {
private:
    size_t capacity;
    std::vector<NetworkSocket> memory_pool; // Contiguous memory block (The Slab)
    std::vector<size_t> free_list;          // Tracks available slot indices

public:
    // When the allocator is born, it "pre-warms" all objects
    SocketSlabAllocator(size_t pool_size) : capacity(pool_size) {
        std::cout << "[Slab] Initializing and pre-warming " << capacity << " slots...\n";
        
        memory_pool.resize(capacity);
        
        for (size_t i = 0; i < capacity; ++i) {
            // Construct the object inside the raw slab memory immediately
            memory_pool[i].construct(static_cast<int>(i + 1000));
            // Add the index to our tracking free-list
            free_list.push_back(i);
        }
        std::cout << "[Slab] Pre-warming complete. All objects built.\n\n";
    }

    // Allocate: Instantly returns a pointer to a pre-warmed object (O(1))
    NetworkSocket* alloc_object() {
        if (free_list.empty()) {
            std::cout << "[Slab] Error: Out of memory slots!\n";
            return nullptr;
        }

        // Pop an index from the free list
        size_t available_idx = free_list.back();
        free_list.pop_back();

        // Fetch the pre-constructed object and activate it
        NetworkSocket* obj = &memory_pool[available_idx];
        obj->mark_active();
        
        return obj;
    }

    // Deallocate: Marks the slot as dormant without running a destructor (O(1))
    void free_object(NetworkSocket* obj) {
        // Calculate the index based on pointer arithmetic
        size_t index = obj - &memory_pool[0];
        
        // Put it to sleep, keeping its internal allocations/structures intact
        memory_pool[index].mark_dormant();
        
        // Return index back to the free list for immediate reuse
        free_list.push_back(index);
    }
};

// 3. TESTING THE PERFORMANCE
int main() {
    // Phase 1: Heavy setup cost happens here
    auto start_init = std::chrono::high_resolution_clock::now();
    SocketSlabAllocator allocator(3); // Creates 3 pre-warmed sockets
    auto end_init = std::chrono::high_resolution_clock::now();
    std::cout << "Setup Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_init - start_init).count() 
              << " ms\n\n";

    // Phase 2: Lightning-fast allocations
    std::cout << "--- Requesting Objects (Instant) ---\n";
    
    auto start_alloc = std::chrono::high_resolution_clock::now();
    NetworkSocket* socketA = allocator.alloc_object();
    NetworkSocket* socketB = allocator.alloc_object();
    auto end_alloc = std::chrono::high_resolution_clock::now();

    std::cout << "Allocated Socket A ID: " << socketA->socket_id << "\n";
    std::cout << "Allocated Socket B ID: " << socketB->socket_id << "\n";
    std::cout << "Time taken to allocate 2 objects: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end_alloc - start_alloc).count() 
              << " microseconds!\n\n";

    // Phase 3: Instant Deallocation (Destructor skipped!)
    std::cout << "--- Freeing Object (Instant, No Destructor) ---\n";
    allocator.free_object(socketA);
    std::cout << "Socket A returned to free list.\n\n";

    // Phase 4: Re-allocating the same slot instantly
    std::cout << "--- Re-allocating Same Slot ---\n";
    NetworkSocket* socketC = allocator.alloc_object();
    std::cout << "Allocated Socket C ID (Reused Slot): " << socketC->socket_id << "\n";

    return 0;
}
