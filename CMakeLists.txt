cmake_minimum_required(VERSION 3.12)

project(math_solver C)
set(CMAKE_C_STANDARD 11)

add_executable(math_solver
	src/main.c
 "include/main.h"
)

target_include_directories(math_solver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)