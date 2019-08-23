cmake_minimum_required(VERSION 3.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/aktualizr/src/libaktualizr)

add_subdirectory("aktualizr")
add_subdirectory("src")
