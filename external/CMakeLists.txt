#glad
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# glfw (On MacOSX pls use Clang to compiile)
set( ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "Build basic example applications" )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory(glfw)

#Eigen Header-Only
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE 
    eigen
    eigen/unsupported
)

#spdlog Header-Only
set(SPDLOG_MASTER_PROJECT OFF CACHE BOOL "" FORCE)
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE
    spdlog/include
)

set(opengl  PARENT_SCOPE)
# Handle OpenGL libraries on MacOS and Windows
if(APPLE)
    # Extra libraries for OpenGL
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(opengl ${opengl} ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
elseif(WIN32)
    # load system opengl library
    find_package(OpenGL REQUIRED)
    set(opengl ${opengl} opengl32)
endif()

# Assimp
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})
find_package(ASSIMP REQUIRED)
set(assimp_includes ${ASSIMP_INCLUDE_DIR} PARENT_SCOPE)
set(assimp ${ASSIMP_LIBRARIES} PARENT_SCOPE)

#imgui
add_library(imgui
    ./imgui/imgui.cpp
    ./imgui/imgui.h
    ./imgui/imgui_demo.cpp
    ./imgui/imgui_draw.cpp
    ./imgui/imgui_tables.cpp
    ./imgui/imgui_internal.h
    ./imgui/imgui_widgets.cpp
    ./imgui/imstb_rectpack.h
    ./imgui/imstb_textedit.h
    ./imgui/imstb_truetype.h
    ./imgui/backends/imgui_impl_glfw.cpp
    ./imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ./imgui)
target_include_directories(imgui PUBLIC ./imgui/backends)
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glad glfw)