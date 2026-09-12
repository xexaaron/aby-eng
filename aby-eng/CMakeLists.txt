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
    include/aby-eng/core/renderer.hpp
    include/aby-eng/core/object.hpp
    include/aby-eng/core/text.hpp
    
    include/aby-eng/misc/types.hpp
    include/aby-eng/misc/uuid.hpp
    include/aby-eng/misc/time.hpp
    include/aby-eng/misc/font.hpp
    include/aby-eng/misc/utf8.hpp
    include/aby-eng/misc/registry.hpp

    include/aby-eng/ui/element.hpp
    include/aby-eng/ui/uitypes.hpp
    include/aby-eng/ui/image.hpp
    include/aby-eng/ui/button.hpp
    include/aby-eng/ui/window.hpp
    include/aby-eng/ui/containers/canvas.hpp
    include/aby-eng/ui/containers/container.hpp
    include/aby-eng/ui/containers/grid.hpp
    include/aby-eng/ui/containers/aspectratiobox.hpp

    src/common.cpp
    src/log.cpp
    
    src/core/app.cpp
    src/core/entry.cpp
    src/core/renderer.cpp
    src/core/object.cpp
    src/core/text.cpp
    
    src/misc/types.cpp
    src/misc/uuid.cpp
    src/misc/time.cpp
    src/misc/font.cpp
    src/misc/utf8.cpp
    src/misc/registry.cpp

    src/ui/element.cpp
    src/ui/uitypes.cpp
    src/ui/image.cpp
    src/ui/button.cpp
    src/ui/window.cpp
    src/ui/containers/canvas.cpp
    src/ui/containers/container.cpp
    src/ui/containers/grid.cpp
    src/ui/containers/aspectratiobox.cpp
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
    glm::glm
    freetype
    argparse::argparse
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/ui.frag
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.frag

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/ui.vert
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.vert

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/default_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default_texture.png

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/Cobblestone.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Cobblestone.png

    COMMENT "[aby:eng] Copying resources"
)