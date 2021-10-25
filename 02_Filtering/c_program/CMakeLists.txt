cmake_minimum_required(VERSION 3.16)
project(pdm_filter LANGUAGES C VERSION 0.0.1)


set(CMAKE_C_STANDARD 11)

configure_file(inc/config.h.in inc/config.h)

include_directories(${PROJECT_BINARY_DIR}/inc/ inc/ lib/pdm_fir)

add_executable(pdm_filter lib/pdm_fir/pdm_fir.c src/main.c)
