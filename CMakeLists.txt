cmake_minimum_required(VERSION 3.17)
project(OpenGLRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gloabl configuration
configure_file(src/Config/Config.h.in src/Config/Config.h)
include_directories(${CMAKE_BINARY_DIR}/src/Config)

# External libraries
add_subdirectory(external)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    ${assimp_includes}
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

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
# imgui contains assimp
target_link_libraries(${PROJECT_NAME} imgui glad glfw spdlog eigen ${opengl} ${assimp_libs})

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/external/assimp-static/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()