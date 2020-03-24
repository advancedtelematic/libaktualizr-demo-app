set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME libaktualizr-demo-app)

set(SOURCES main.cc)

add_executable(${TARGET_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)
target_link_libraries(${TARGET_NAME} aktualizr_lib)

install(TARGETS ${TARGET_NAME} DESTINATION bin)
