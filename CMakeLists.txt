cmake_minimum_required(VERSION 3.31)
project(final_state_transducer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(src)

add_executable(Executable src/main.cpp)
