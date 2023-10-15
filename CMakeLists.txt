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

# parse_mul_implicit
add_executable(parse_mul_implicit tests/parse_mul_implicit.c)
target_link_libraries(parse_mul_implicit PRIVATE math_solver)
add_test(
  NAME parse_mul_implicit
  COMMAND $<TARGET_FILE:parse_mul_implicit>
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

# parse_ge
add_executable(parse_ge tests/parse_ge.c)
target_link_libraries(parse_ge PRIVATE math_solver)
add_test(
  NAME parse_ge
  COMMAND $<TARGET_FILE:parse_ge>
)

# parse_le
add_executable(parse_le tests/parse_le.c)
target_link_libraries(parse_le PRIVATE math_solver)
add_test(
  NAME parse_le
  COMMAND $<TARGET_FILE:parse_le>
)

# parse_neq
add_executable(parse_neq tests/parse_neq.c)
target_link_libraries(parse_neq PRIVATE math_solver)
add_test(
  NAME parse_neq
  COMMAND $<TARGET_FILE:parse_neq>
)

# parse_number
add_executable(parse_number tests/parse_number.c)
target_link_libraries(parse_number PRIVATE math_solver)
add_test(
  NAME parse_number
  COMMAND $<TARGET_FILE:parse_number>
)

# parse_variable
add_executable(parse_variable tests/parse_variable.c)
target_link_libraries(parse_variable PRIVATE math_solver)
add_test(
  NAME parse_variable
  COMMAND $<TARGET_FILE:parse_variable>
)

# parse_subscript
add_executable(parse_subscript tests/parse_subscript.c)
target_link_libraries(parse_subscript PRIVATE math_solver)
add_test(
  NAME parse_subscript
  COMMAND $<TARGET_FILE:parse_subscript>
)

# parse_varop
add_executable(parse_varop tests/parse_varop.c)
target_link_libraries(parse_varop PRIVATE math_solver)
add_test(
  NAME parse_varop
  COMMAND $<TARGET_FILE:parse_varop>
)

# format_round
add_executable(format_round tests/format_round.c)
target_link_libraries(format_round PRIVATE math_solver)
add_test(
  NAME format_round
  COMMAND $<TARGET_FILE:format_round>
)

# standardize_implicit_mul
add_executable(standardize_implicit_mul tests/standardize_implicit_mul.c)
target_link_libraries(standardize_implicit_mul PRIVATE math_solver)
add_test(
  NAME standardize_implicit_mul
  COMMAND $<TARGET_FILE:standardize_implicit_mul>
)

# reduce_implicit_mul
add_executable(reduce_implicit_mul tests/reduce_implicit_mul.c)
target_link_libraries(reduce_implicit_mul PRIVATE math_solver)
add_test(
  NAME reduce_implicit_mul
  COMMAND $<TARGET_FILE:reduce_implicit_mul>
)

# inflate_depth
add_executable(inflate_depth tests/inflate_depth.c)
target_link_libraries(inflate_depth PRIVATE math_solver)
add_test(
  NAME inflate_depth
  COMMAND $<TARGET_FILE:inflate_depth>
)

# inflate_round
add_executable(inflate_round tests/inflate_round.c)
target_link_libraries(inflate_round PRIVATE math_solver)
add_test(
  NAME inflate_round
  COMMAND $<TARGET_FILE:inflate_round>
)

target_include_directories(math_solver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)