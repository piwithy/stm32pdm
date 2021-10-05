cmake_minimum_required(VERSION 3.16)
project(pdm_filtering C)

set(CMAKE_C_STANDARD 11)

include_directories(inc/ lib/fir_filter)

add_executable(pdm_filter lib/fir_filter/pdm_fir.c src/main.c)
