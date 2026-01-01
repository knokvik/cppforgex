# CppForgeX

This repository serves as a comprehensive guide for modern C++ features. It details critical conditions, common pitfalls, and their respective solutions to ensure robust and secure code development.

## Contribution Chart

*(Note: The commit history in this repository has been generated to demonstrate consistent contribution activity.)*

## Resources List

- [C++ Reference Documentation](https://en.cppreference.com/)
- [CMake Official Documentation](https://cmake.org/documentation/)
- [Modern C++ Features Overview](https://github.com/AnthonyCalandra/modern-cpp-features)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

---

## Critical Concepts Covered

### 1. The Diamond Problem (Object-Oriented Programming)
**File:** `oop/oop.cpp`

**Description:** Occurs in multiple inheritance when two classes inherit from a common base class, and a fourth class inherits from both. This causes ambiguity and duplicate instances of the base class.
**Solution:** Utilize virtual inheritance (`virtual public`) to ensure a single, shared instance of the base class is maintained.

### 2. Deadlocks (Multithreading)
**File:** `threads/thread.cpp`

**Description:** A system freeze that occurs when two or more threads wait indefinitely for each other to release locks, preventing further execution.
**Solution:** Acquire multiple locks in a consistent order across all threads. In modern C++, utilize `std::scoped_lock` to safely acquire multiple mutexes simultaneously without causing deadlocks.

### 3. Dangling Pointers & Memory Management
**File:** `pointers/ptr.cpp`

**Description:** A dangling pointer references memory that has been freed or deleted. Dereferencing it leads to undefined behavior, memory corruption, and potential security vulnerabilities.
**Solution:** Adopt modern C++ smart pointers (`std::unique_ptr` and `std::shared_ptr`) provided by the `<memory>` header to manage dynamic memory allocation safely and automatically.

### 4. Iterator Invalidation & Bounds Checking
**File:** `arrays/arr.cpp`

**Description:** Iterator invalidation occurs when structural changes to a container (like `std::vector`) cause stored iterators to point to freed memory. Accessing elements out of bounds leads to silent memory corruption.
**Solution:** Re-assign iterators after modifying capacity. Always use the `.at()` method for element access when bounds are uncertain, as it includes bounds checking and throws an exception on violation.

### 5. Dangling std::string_view
**File:** `strings/str.cpp`

**Description:** `std::string_view` provides a read-only view into a string without ownership. Returning or storing a `std::string_view` to a temporary string results in a dangling reference once the temporary string is destroyed.
**Solution:** Restrict the use of `std::string_view` to function parameters or scenarios where the underlying string's lifetime strictly exceeds that of the view.

---
*End of Document*
