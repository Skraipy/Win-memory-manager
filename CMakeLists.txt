cmake_minimum_required(VERSION 3.10)
project(win-memory-manager VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(memory_manager src/Win_memory_manager.cpp)

if(WIN32)
	target_link_libraries(memory_manager)
	target_compile_options(memory_manager PRIVATE "/utf-8")
    target_compile_definitions(memory_manager PRIVATE "_UNICODE" "UNICODE")
endif()  