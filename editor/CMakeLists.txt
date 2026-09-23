cmake_minimum_required(VERSION 3.28.3)
project(aby-eng-editor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(${PROJECT_NAME}
    include/main.hpp
    include/widgets/editor_console.hpp
    include/widgets/editor_settings.hpp
    include/widgets/editor_logs.hpp

    src/main.cpp
    src/widgets/editor_console.cpp
    src/widgets/editor_settings.cpp
    src/widgets/editor_logs.cpp

    ${VENDOR_DIR}/QAnsiTextEdit/src/QAnsiTextEdit.h
    ${VENDOR_DIR}/QAnsiTextEdit/src/ANSI-color-codes.h
    ${VENDOR_DIR}/QAnsiTextEdit/src/QAnsiTextEdit.cpp
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    include
    ${VENDOR_DIR}/QAnsiTextEdit/src
)

target_link_libraries(${PROJECT_NAME}
PRIVATE 
    aby-eng::aby-eng
    Qt6::Widgets
    qtermwidget6
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

