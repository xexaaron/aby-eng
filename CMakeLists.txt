cmake_minimum_required(VERSION 3.28.3)
project(aby-eng LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

add_subdirectory(${VENDOR_DIR}/aby-rhi)
add_subdirectory(${VENDOR_DIR}/aby-win)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${VENDOR_DIR}/glm)

set(FT_REQUIRE_ZLIB TRUE CACHE BOOL "" FORCE)
set(FT_REQUIRE_BZIP2 TRUE CACHE BOOL "" FORCE)
set(FT_REQUIRE_PNG TRUE CACHE BOOL "" FORCE)
set(FT_REQUIRE_HARFBUZZ TRUE CACHE BOOL "" FORCE)
set(FT_REQUIRE_BROTLI TRUE CACHE BOOL "" FORCE)
add_subdirectory(${VENDOR_DIR}/freetype)

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

set_target_properties(glm PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/aby-eng)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
