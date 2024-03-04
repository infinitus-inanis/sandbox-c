set(ii_name ii)
add_library(${ii_name} STATIC)
target_sources(${ii_name}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ii-utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ii-array.c
)
target_include_directories(${ii_name}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)