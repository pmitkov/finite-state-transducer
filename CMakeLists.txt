# Build
cmake_minimum_required(VERSION 3.31)
project(final_state_transducer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
include_directories(src)
add_executable(REPL src/main.cpp)

# Test setup
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)
enable_testing()
file(GLOB TEST_SOURCES "test/*.test.cpp")
add_executable(UnitTests ${TEST_SOURCES})
target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(UnitTests)

# Test files
file(GLOB TEST_FILES "${CMAKE_SOURCE_DIR}/test/*.txt")
add_custom_target(copy_files ALL)
foreach(file ${TEST_FILES})
    get_filename_component(filename ${file} NAME)
    add_custom_command(TARGET copy_files PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${file}
            ${CMAKE_BINARY_DIR}/${filename}
    )
endforeach()