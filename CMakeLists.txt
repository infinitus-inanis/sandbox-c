cmake_minimum_required(VERSION 3.22.1)
project(algo LANGUAGES C)

option(ASAN_STATIC "enable address sanitizer" OFF)

add_executable(${PROJECT_NAME} main.c)

target_compile_options(${PROJECT_NAME} PUBLIC
  -Wall -Wextra -Wconversion
  -O0 -g -ggdb -gdwarf 
)

if (ASAN_STATIC)
  target_compile_options(${PROJECT_NAME} PUBLIC
    -fsanitize=address -fno-omit-frame-pointer -static-libasan
  )
  target_link_libraries(${PROJECT_NAME} PUBLIC
    libasan.a m
  )
endif()

target_sources(${PROJECT_NAME}
  PUBLIC
    ii/ii-stdext.c
    ii/ii-array.c
)

target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}
    ii/
)