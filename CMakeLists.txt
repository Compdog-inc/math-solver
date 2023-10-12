cmake_minimum_required(VERSION 3.12)

project(math_solver C)
set(CMAKE_C_STANDARD 11)

# library
add_library(math_solver STATIC src/mathsolver.c)

# main code
add_executable(main src/main.c)
target_link_libraries(main PRIVATE math_solver)

# enable testing functionality
enable_testing()

# parse_mul
add_executable(parse_mul tests/parse_mul.c)
target_link_libraries(parse_mul PRIVATE math_solver)
add_test(
  NAME parse_mul
  COMMAND $<TARGET_FILE:parse_mul>
)

# parse_mul_complex
add_executable(parse_mul_complex tests/parse_mul_complex.c)
target_link_libraries(parse_mul_complex PRIVATE math_solver)
add_test(
  NAME parse_mul_complex
  COMMAND $<TARGET_FILE:parse_mul_complex>
)

# parse_div
add_executable(parse_div tests/parse_div.c)
target_link_libraries(parse_div PRIVATE math_solver)
add_test(
  NAME parse_div
  COMMAND $<TARGET_FILE:parse_div>
)

# parse_div_complex
add_executable(parse_div_complex tests/parse_div_complex.c)
target_link_libraries(parse_div_complex PRIVATE math_solver)
add_test(
  NAME parse_div_complex
  COMMAND $<TARGET_FILE:parse_div_complex>
)

# parse_add
add_executable(parse_add tests/parse_add.c)
target_link_libraries(parse_add PRIVATE math_solver)
add_test(
  NAME parse_add
  COMMAND $<TARGET_FILE:parse_add>
)

# parse_add_complex
add_executable(parse_add_complex tests/parse_add_complex.c)
target_link_libraries(parse_add_complex PRIVATE math_solver)
add_test(
  NAME parse_add_complex
  COMMAND $<TARGET_FILE:parse_add_complex>
)

# parse_sub
add_executable(parse_sub tests/parse_sub.c)
target_link_libraries(parse_sub PRIVATE math_solver)
add_test(
  NAME parse_sub
  COMMAND $<TARGET_FILE:parse_sub>
)

# parse_sub_complex
add_executable(parse_sub_complex tests/parse_sub_complex.c)
target_link_libraries(parse_sub_complex PRIVATE math_solver)
add_test(
  NAME parse_sub_complex
  COMMAND $<TARGET_FILE:parse_sub_complex>
)

target_include_directories(math_solver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)