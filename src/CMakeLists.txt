cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(MccHatServer CXX)

file(GLOB SRC_ALL ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB HDR_ALL ${PROJECT_SOURCE_DIR}/*.h)
foreach(HDR ${HDR_ALL})
    string(REGEX REPLACE "[.]h$" ".cc" SRC ${HDR})
    set(SRC_EXTRA ${SRC_EXTRA} ${SRC})
endforeach()
#message("SRC_EXTRA: ${SRC_EXTRA}")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall -I/usr/local/include")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib -ldaqhats")
#list(APPEND CMAKE_INSTALL_RPATH "/usr/local/lib")

add_executable(mcc-hat-server
	${PROJECT_SOURCE_DIR}/mcc-hat-server.cc ${SRC_EXTRA}
)
target_link_libraries(mcc-hat-server -L/usr/local/lib -ldaqhats)
install(TARGETS mcc-hat-server  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

add_executable(mcc-hat-client
	${PROJECT_SOURCE_DIR}/mcc-hat-client.cc ${SRC_EXTRA}
)
target_link_libraries(mcc-hat-client  -L/usr/local/lib -ldaqhats)
install(TARGETS mcc-hat-client  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
