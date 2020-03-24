cmake_minimum_required(VERSION 3.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/aktualizr/src/libaktualizr)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/aktualizr/third_party/jsoncpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/aktualizr/third_party/googletest/googletest/include)
add_subdirectory("aktualizr")
add_subdirectory("src")
