cmake_minimum_required(VERSION 3.17)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gloabl configuration
configure_file(src/Config/Config.h.in src/Config/Config.h)
include_directories(${CMAKE_BINARY_DIR}/src/Config)

# External libraries
add_subdirectory(external)

# Handle some special libraries on MacOS and Windows
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
    # load system opengl library
    find_package(OpenGL REQUIRED)
    set(libs ${libs} opengl32)
endif()

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(main ${libs} imgui glad glfw spdlog eigen)
