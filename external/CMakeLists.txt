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

#imgui
set(IMGUI_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

list(APPEND IMGUI_PUBLIC_HEADERS
    "${IMGUI_SRC_DIR}/imgui.h"
)

list(APPEND IMGUI_PRIVATE_HEADERS
    "${IMGUI_SRC_DIR}/imgui_internal.h"
    "${IMGUI_SRC_DIR}/imconfig.h"
    "${IMGUI_SRC_DIR}/stb_rect_pack.h"
    "${IMGUI_SRC_DIR}/stb_textedit.h"
    "${IMGUI_SRC_DIR}/stb_truetype.h"
)

list(APPEND IMGUI_SOURCES
    "${IMGUI_SRC_DIR}/imgui.cpp"
    "${IMGUI_SRC_DIR}/imgui_draw.cpp"
    "${IMGUI_SRC_DIR}/imgui_demo.cpp"
    "${IMGUI_SRC_DIR}/imgui_widgets.cpp"
)

list(APPEND IMGUI_PUBLIC_INCLUDE_DIRS
    ${IMGUI_SRC_DIR}
)

list(APPEND IMGUI_PUBLIC_LIBRARIES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLAD")
add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC ${IMGUI_PUBLIC_LIBRARIES})
target_include_directories(imgui
    PUBLIC ${IMGUI_PUBLIC_INCLUDE_DIRS}
    PRIVATE ${IMGUI_PRIVATE_INCLUDE_DIRS}
)

#Eigen Header-Only
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE 
    eigen
    eigen/unsupported
)

#spdlog Header-Only
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE
    spdlog/include
)