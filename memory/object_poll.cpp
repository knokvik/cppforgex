#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

class Particle {
public:
    int x = 0, y = 0;
    void reset() { x = 0; y = 0; }
    void update() { x++; y++; }
};

class ParticlePool {
private:
    std::vector<Particle> m_storage;
    std::vector<size_t> m_availablePool;

public:
    ParticlePool(size_t maxCapacity) {
        m_storage.resize(maxCapacity);
        m_availablePool.reserve(maxCapacity);
        for (size_t i = 0; i < maxCapacity; ++i) {
            m_availablePool.push_back(i);
        }
    }

    struct PoolReturner {
        ParticlePool* pool;
        size_t index;
        
        void operator()(Particle* p) const {
            p->reset();
            pool->returnToPool(index);
            std::cout << "[Pool] Slot " << index << " returned safely to recycling.\n";
        }
    };

    using PoolPtr = std::unique_ptr<Particle, PoolReturner>;

    PoolPtr borrowObject() {
        if (m_availablePool.empty()) {
            throw std::runtime_error("Pool is completely empty! Out of memory slots.");
        }

        size_t index = m_availablePool.back();
        m_availablePool.pop_back();

        std::cout << "[Pool] Borrowed Slot " << index << "\n";
        return PoolPtr(&m_storage[index], PoolReturner{this, index});
    }

private:
    void returnToPool(size_t index) {
        m_availablePool.push_back(index);
    }
};

int main() {
    ParticlePool pool(3);

    {
        std::cout << "--- Main Loop Step 1 ---\n";
        auto p1 = pool.borrowObject();
        auto p2 = pool.borrowObject();
        p1->update();
    }

    std::cout << "\n--- Main Loop Step 2 ---\n";
    auto p3 = pool.borrowObject();
    auto p4 = pool.borrowObject();

    return 0;
}
// Refactored variables
