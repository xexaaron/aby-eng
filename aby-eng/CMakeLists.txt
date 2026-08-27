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
    include/aby-eng/log.hpp
    include/aby-eng/core/app.hpp
    include/aby-eng/core/entry.hpp
    src/common.cpp
    src/log.cpp
    src/core/app.cpp
    src/core/entry.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
PUBLIC
    include/
    include/aby-eng
PRIVATE
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    aby-win::aby-win
    aby-rhi::aby-rhi
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ABY_BUILD_DLL=1
    ABY_ENG_ENABLE_ASSERT=1
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX
        /Zc:preprocessor
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)