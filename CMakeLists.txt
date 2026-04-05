cmake_minimum_required(VERSION 3.30.5)
project(Search VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)


add_executable(search 
    src/main.cpp
    src/core/crawler.cpp
    src/core/parser/parserhelper.cpp
)

target_link_libraries(search PRIVATE CURL::libcurl Python3::Python)
target_include_directories(search PRIVATE src/core)
