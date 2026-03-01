cmake_minimum_required(VERSION 3.30.5)
project(Search VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)



add_executable(search 
    src/main.cpp
    src/core/crawler.cpp
)

target_link_libraries(search PRIVATE CURL::libcurl)
target_include_directories(search PRIVATE src/core)
