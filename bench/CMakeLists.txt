cmake_minimum_required(VERSION 3.16)

project(PerfLab)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(google-benchmark)

add_executable(vector_bench benches/vector_bench.cpp)

target_link_libraries(vector_bench PRIVATE benchmark::benchmark benchmark::benchmark_main)
