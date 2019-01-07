cmake_minimum_required(VERSION 3.1)

project(RAWTX-DEMO)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_DEBUG_POSTFIX "-dbg")

# subdirectories
add_subdirectory(src)
