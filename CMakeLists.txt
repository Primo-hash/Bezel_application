# Setting required cmake version
cmake_minimum_required(VERSION 3.15)

# Project definition
set(SANDBOX_VERSION "0.0.1")
project(
  Pacman
  VERSION ${SANDBOX_VERSION}
  LANGUAGES CXX)

find_package(OpenGL REQUIRED)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # For exporting in windows
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # For static libraries (spdlog) included
                                         # by shared libraries

# Build output structure
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Initialize git submodules
find_package(Git REQUIRED)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")

    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


# Dependencies
add_subdirectory(vendor/engine)


# APP EXECUTABLE
# Create the executable
# Change from pacman/pacman.cpp to sandbox/sandbox.cpp to compile sandbox env
# Executable will always be named after project name
add_executable(${PROJECT_NAME} pacman/pacman.cpp
    "pacman/include/characters/ghost.h"
    "pacman/include/characters/pacman.h"
    "pacman/include/inanimate-objects/wall.h"
    "pacman/include/inanimate-objects/map.h"
    "pacman/include/layers/game-layer.h"
    "pacman/include/color.h" "pacman/include/inanimate-objects/pellet.h"
    "pacman/include/logic/collision.h")


# Engine is the Engine .lib file The rest of linked libraries are there
# for convenience since the engine utilizes them.
target_link_libraries(
  ${PROJECT_NAME}
  Engine
  glad
  glfw
  glm
  spdlog
  stb
  tinyobjloader
  OpenGL::GL)

  
# OPTIONS
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(default_build_type "Release")

include_directories(${PROJECT_NAME} INTERFACE 
	${CMAKE_CURRENT_LIST_DIR}
)

if (WIN32 OR CYGWIN)
    # WINDOWS AND CYGWIN COMPILER
    # Runs executable in multiple threads
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif (WIN32 OR CYGWIN)


# Moving all assets with executable for "sensible" access
add_custom_command(
# Assets to be copied to build folder
TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_LIST_DIR}/pacman/assets
${CMAKE_CURRENT_BINARY_DIR}/bin/assets

# Assets to be copied to build folder
TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_LIST_DIR}/sandbox/assets
${CMAKE_CURRENT_BINARY_DIR}/bin/assets
)