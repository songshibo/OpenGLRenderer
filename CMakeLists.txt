cmake_minimum_required(VERSION 3.17)
project(Renderer)
set(CMAKE_CXX_STANDARD 17)

configure_file(src/Config/Config.h.in src/Config/Config.h)
include_directories(${CMAKE_BINARY_DIR}/src/Config)

set(libs )
if(APPLE)
    # Extra libraries for OpenGL
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(libs ${libs} ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
elseif(WIN32)
    find_package(OpenGL REQUIRED)
    set(libs ${libs} opengl32)
endif()

add_subdirectory(external)
add_executable(main src/main.cpp)

target_link_libraries(main ${libs} glad imgui glfw spdlog eigen)
