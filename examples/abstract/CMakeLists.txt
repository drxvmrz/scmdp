cmake_minimum_required(VERSION 3.5.0)

project(scmdp_calc)

add_executable(app src/main.c)

include_directories(inc)
add_library(scmdp inc/scmdp.h src/scmdp.c)

target_link_libraries(app scmdp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

if (MSVC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build)

endif()