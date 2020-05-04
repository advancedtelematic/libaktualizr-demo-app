set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME libaktualizr-demo-app)

set(SOURCES main.cc example_pacman.cc)

find_program(CLANG_TIDY_BIN "clang-tidy-6.0")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/aktualizr/cmake-modules")
find_package(GLIB2 REQUIRED)
find_package(OSTree REQUIRED)

if (RUN_CHECKS)
  if (CLANG_TIDY_BIN)
    set(CMAKE_CXX_CLANG_TIDY
      ${CLANG_TIDY_BIN} ;
      -checks=*,-fuchsia-*;
      -warnings-as-errors=*;)
  else()
    message(WARNING "clang-tidy not found")
  endif()
endif()

add_executable(${TARGET_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${TARGET_NAME} PRIVATE ${GLIB2_INCLUDE_DIRS} ${LIBOSTREE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/aktualizr/src/virtual_secondary)
target_link_libraries(${TARGET_NAME} aktualizr_lib virtual_secondary)

install(TARGETS ${TARGET_NAME} DESTINATION bin)
