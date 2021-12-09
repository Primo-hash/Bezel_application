# Setting required cmake version
cmake_minimum_required(VERSION 3.15)

# Project definition
set(ENGINE_VERSION "0.0.1")
project(
  engine
  VERSION ${ENGINE_VERSION}
  LANGUAGES CXX)

# Installs directories by GNU standards
# Linux file variables works on Windows, Windows doesnt work on linux
include(GNUInstallDirs)
find_package(OpenGL REQUIRED)

# Stops GLFW from compiling test executables
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Dependencies
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/tinyobj)

# Scope
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ../)


# Precompiled headers
target_precompile_headers(${PROJECT_NAME} INTERFACE "precompiled.h")

# Engine is a static library instead of dynamic because the more complete the engine becomes the less changes will be made to the code,
# and so it would make more sense for the application using the Engine to be a dynamic library to its components and have the engine create
# the executable instead since changes are always made on the application.

add_library(Engine STATIC
	# We are aware of the usage of GLOB to add files by regex
	# However it is strongly discouraged because the build system won't know
	# when to ask CMake to regenerate in some cases.
	# That is why we have chosen to list manually.

	# ./include
	"include/entrypoint.h" "include/app-frame.h" "include/logger.h" "include/core.h"
	"include/window/window.h" "include/time.h" "include/layer.h" "include/input.h"

	# ./include/events
	"include/events/event.h" "include/events/key-event.h" "include/events/app-event.h" "include/events/mouse-event.h"

	# ./include/graphics
	"include/graphics/buffer.h" "include/graphics/vertex-array.h" "include/graphics/shader.h" 
	"include/graphics/texture.h" "include/graphics/renderer.h" "include/graphics/renderAPI.h"
	"include/graphics/object-library.h" "include/graphics/3D-processing/mesh-data.h"
	"include/graphics/storage.h"

	# ./include/graphics/camera
	"include/graphics/camera/camera-controller.h" "include/graphics/camera/orthographic-camera.h"
	"include/graphics/camera/perspective-camera.h"

	# ./include/window
	"include/window/window.h" "include/window/window-context.h"

	# ./src
	"src/app-frame.cpp" "src/logger.cpp" "src/window.cpp" "src/window-context.cpp" "src/window.cpp"
	"src/layer.cpp" "src/input.cpp" "src/buffer.cpp" "src/vertex-array.cpp" "src/shader.cpp" 
	"src/orthographic-camera.cpp" "src/camera-controller.cpp" "src/texture.cpp" "src/renderer.cpp"
	"src/renderAPI.cpp" "src/perspective-camera.cpp" "src/object-library.cpp" 
	"src/mesh-data.cpp"

	# ./
	"engine.h"
	)


# Links libraries to engine
target_link_libraries(Engine PUBLIC 
	${PROJECT_NAME}
	glad
	glfw
	glm
	spdlog
	stb
	tinyobjloader
	OpenGL::GL)

# Interface library needs an alias, works like "Creating an object for a class"
add_library(engine::Engine ALIAS ${PROJECT_NAME})
target_compile_definitions(Engine PUBLIC GLFW_INCLUDE_NONE ENABLE_ASSERTS)	# Universal flags

# Compiler/Platform specific flags
if (WIN32 OR CYGWIN)
    # WINDOWS AND CYGWIN COMPILER
	target_compile_definitions(Engine PUBLIC PLATFORM_WINDOWS)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")	# Check build type for multithreading application
		target_compile_options(Engine PRIVATE /EHsc /MDd /c /MP)	# Multithreading for libraries
	else()
		target_compile_options(Engine PRIVATE /EHsc /MD /c /MP)	# Multithreading for libraries
	endif()
endif (WIN32 OR CYGWIN)
if (UNIX)
    # UNIX (including MAC OS X)
	target_compile_definitions(Engine PUBLIC PLATFORM_UNIX)
    target_compile_options(Engine PRIVATE -Wall -Wextra -fPIC)	# Error mitigation flags
endif (UNIX)

# DLL exporting specific steps
set_target_properties(Engine PROPERTIES PUBLIC_HEADER 
	include/engine.h
)

# C++ 17 required
set_property(TARGET Engine PROPERTY CXX_STANDARD 17)