cmake_minimum_required(VERSION 3.5.0)

project(scmdp_calc)

add_executable(app src/main.c)

include_directories(inc)
add_library(scmdp inc/scmdp.h src/scmdp.c)

target_link_libraries(app scmdp)
