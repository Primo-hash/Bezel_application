cmake_minimum_required(VERSION 3.15)
project(stb)

set(SOURCE_FILES "stb_image.cpp" "src/stb_image.h")
add_library(stb STATIC ${SOURCE_FILES})

target_link_libraries( ${PROJECT_NAME}
  engine
  )
