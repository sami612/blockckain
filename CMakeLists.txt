cmake_minimum_required(VERSION 3.9)

project(
  blockchain
  VERSION 1.0.0
  DESCRIPTION "Reimplementation of bitcoin in C"
  LANGUAGES C)

file(GLOB_RECURSE sources src/*.c src/*.h)

add_compile_options(
  -O3
  -march=native
  -Wno-unused-parameter
  -m64
  -std=c99
  -pedantic
  -Wall
  -Wextra
  -Werror
  -Wshadow
  -Wdouble-promotion
  -Wformat=2
  -Wpointer-arith
  -Winit-self
  -Wcast-align
  -Wcast-qual
  -Wunreachable-code
  -Wformat-truncation
  -Wmissing-prototypes
  -Wundef
  -fno-common)

add_executable(${CMAKE_PROJECT_NAME} ${sources})
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
  m
  ssl
  crypto
)

find_package(OpenSSL REQUIRED)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES C_STANDARD 99)
