cmake_minimum_required(VERSION 3.5.0)

project(scmdp_calc)

add_executable(CALC src/main.c)

include_directories(inc)
add_library(scmdp inc/scmdp.h src/scmdp.c)

target_link_libraries(CALC scmdp)
