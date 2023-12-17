cmake_minimum_required(VERSION 3.10)
project(intrin C)

# This option optimizes the code for speed at runtime
# add_compile_options(-O3)
add_compile_options(-msse -mavx)

include_directories(include)
add_executable(intrin src/intrin.c)