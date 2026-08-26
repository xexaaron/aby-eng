cmake_minimum_required(VERSION 3.28.3)
project(aby-eng LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
 
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

add_subdirectory(${VENDOR_DIR}/aby-rhi)
add_subdirectory(${VENDOR_DIR}/aby-win)

set_target_properties(aby-win PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

set_target_properties(glfw PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

set_target_properties(SDL3-static PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

set_target_properties(aby-win-test PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/aby-eng)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
