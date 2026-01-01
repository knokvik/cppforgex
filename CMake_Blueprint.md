# -------------------------------------------------------------------
# 1. Setup & Project Metadata
# -------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20...3.30) # Always define a range
project(EngineMasterclass 
    VERSION 1.0.0 
    LANGUAGES CXX
)

# Enforce Modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# 2. Define a Library Target (The 'Math' Component)
# -------------------------------------------------------------------
add_library(MyMath STATIC
    src/math/calculator.cpp
)

# Tell the world where this library's headers live
target_include_directories(MyMath
    PUBLIC  include/math
    PRIVATE src/math/detail
)

# -------------------------------------------------------------------
# 3. Define the Executable Target (The App)
# -------------------------------------------------------------------
add_executable(GameApp 
    src/main.cpp
)

# -------------------------------------------------------------------
# 4. Dependency Injection (The Magic Step)
# -------------------------------------------------------------------
# This automatically inherits MyMath's compiled code AND its PUBLIC include directories
target_link_libraries(GameApp 
    PRIVATE MyMath
)

# -------------------------------------------------------------------
# 5. Compiler Flags & Warnings (Keep it clean)
# -------------------------------------------------------------------
if(MSVC)
    target_compile_options(GameApp PRIVATE /W4 /WX)
else()
    target_compile_options(GameApp PRIVATE -Wall -Wextra -Werror)
endif()
