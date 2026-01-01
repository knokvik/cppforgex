cmake_minimum_required(VERSION 3.20...3.30)
project(EnginerMasterclass
        VERSION 1.0.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../bench ${CMAKE_CURRENT_BINARY_DIR}/bench_build)

add_executable(EngineMasterclass main.cpp)  

target_link_libraries(
    EngineMasterclass
    PRIVATE
    benchmark::benchmark
)