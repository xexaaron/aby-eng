cmake_minimum_required(VERSION 3.28.3)
project(aby-eng LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

add_library(${PROJECT_NAME} SHARED
    include/aby-eng/common.hpp
    src/common.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
PUBLIC
    include/aby-eng
PRIVATE
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    aby-win::aby-win
    aby-rhi::aby-rhi
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX
        /Zc:preprocessor
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
)