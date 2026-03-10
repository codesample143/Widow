cmake_minimum_required(VERSION 3.10)
project(BenchmarkProject CXX)

# Force C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the directory containing bigdb.hpp
include_directories("../core/bigdb")

# Create the executable
add_executable(benchmark benchmark.cpp)

# Link threading library (required for std::thread on many systems)
find_package(Threads REQUIRED)
target_link_libraries(benchmark Threads::Threads)