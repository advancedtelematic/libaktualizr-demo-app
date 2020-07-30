set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME libaktualizr-demo-app)

set(SOURCES main.cc custom-secondary.cc custom-pacman.cc)

find_program(CLANG_TIDY_BIN "clang-tidy-6.0")

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

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/aktualizr/src/virtual_secondary)
target_link_libraries(${TARGET_NAME} aktualizr_lib virtual_secondary)

install(TARGETS ${TARGET_NAME} DESTINATION bin)
