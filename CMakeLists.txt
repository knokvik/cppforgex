cmake_minimum_required(VERSION 3.10)
project(sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sample 
    main.cpp 
    strings/str.cpp 
    arrays/arr.cpp 
    pointers/ptr.cpp
    oop/oop.cpp
    fstream/file.cpp
    threads/thread.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

target_include_directories(sample PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/strings
    ${CMAKE_CURRENT_SOURCE_DIR}/arrays
    ${CMAKE_CURRENT_SOURCE_DIR}/pointers
    ${CMAKE_CURRENT_SOURCE_DIR}/oop
        ${CMAKE_CURRENT_SOURCE_DIR}/threads
    ${CMAKE_CURRENT_SOURCE_DIR}/fstream
)
