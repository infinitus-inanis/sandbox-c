cmake_minimum_required(VERSION 3.22.1)
project(algo LANGUAGES C)

add_compile_options(
  -Wall -Wextra -Wconversion
  -O0 -g -ggdb -gdwarf
)

option(ASAN_STATIC "enable address sanitizer" OFF)
if (ASAN_STATIC)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -static-libasan)
  link_libraries(libasan.a m)
endif()

add_subdirectory(ii/)
add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}
    ii/
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    lib-ii
)