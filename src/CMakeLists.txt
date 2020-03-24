set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME libaktualizr-demo-app)

set(SOURCES main.cc)

add_executable(${TARGET_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/aktualizr/src/virtual_secondary)
target_link_libraries(${TARGET_NAME} aktualizr_lib virtual_secondary)

install(TARGETS ${TARGET_NAME} DESTINATION bin)
