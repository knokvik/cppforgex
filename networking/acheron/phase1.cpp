#include <memory_resource>
#include <sys/mman.h>
#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <new>
#include <vector>

class HugePageMonotonicResource : public std::pmr::memory_resource
{
public:
    explicit HugePageMonotonicResource(std::size_t bytes)
        : capacity_(bytes)
    {
        memory_ = mmap(
            nullptr,
            capacity_,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE |
            MAP_ANONYMOUS |
            MAP_HUGETLB,
            -1,
            0);

        if (memory_ == MAP_FAILED)
        {
            throw std::bad_alloc{};
        }

        current_ = static_cast<std::byte*>(memory_);
        end_     = current_ + capacity_;
    }

    ~HugePageMonotonicResource() override
    {
        if (memory_ && memory_ != MAP_FAILED)
        {
            munmap(memory_, capacity_);
        }
    }

    HugePageMonotonicResource(
        const HugePageMonotonicResource&) = delete;

    HugePageMonotonicResource& operator=(
        const HugePageMonotonicResource&) = delete;

private:
    void* do_allocate(
        std::size_t bytes,
        std::size_t alignment) override
    {
        alignment = std::max<std::size_t>(alignment, 64);

        void* ptr = current_;
        std::size_t space =
            static_cast<std::size_t>(end_ - current_);

        void* aligned =
            std::align(alignment,
                       bytes,
                       ptr,
                       space);

        if (!aligned)
        {
            throw std::bad_alloc{};
        }

        current_ =
            static_cast<std::byte*>(aligned) + bytes;

        return aligned;
    }

    void do_deallocate(
        void*,
        std::size_t,
        std::size_t) override
    {
        // Monotonic allocator:
        // individual deallocations ignored.
    }

    bool do_is_equal(
        const std::pmr::memory_resource& other)
        const noexcept override
    {
        return this == &other;
    }

private:
    void* memory_{nullptr};

    alignas(64) std::byte* current_{nullptr};
    alignas(64) std::byte* end_{nullptr};

    std::size_t capacity_{0};
};

int main()
{
    constexpr std::size_t HUGE_PAGE_SIZE =
        2 * 1024 * 1024; // 2MB

    try
    {
        HugePageMonotonicResource arena(
            HUGE_PAGE_SIZE);

        std::pmr::vector<int> vec(&arena);

        for (int i = 0; i < 100000; ++i)
        {
            vec.push_back(i);
        }

        std::pmr::string str(&arena);
        str = "Hello PMR + Huge Pages";

        std::cout
            << "Vector size: "
            << vec.size()
            << '\n';

        std::cout
            << "String: "
            << str
            << '\n';

        std::cout
            << "Vector data address: "
            << static_cast<void*>(vec.data())
            << '\n';

        std::cout
            << "Alignment check: "
            << (reinterpret_cast<std::uintptr_t>(
                    vec.data()) %
                64)
            << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Allocation failed: "
            << e.what()
            << '\n';
    }
}