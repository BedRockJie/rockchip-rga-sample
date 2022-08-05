cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)

project(bedrock_rga_test)

include_directories(inc)

set(SOURCES main.c)

add_executable(${PROJECT_NAME} ${SOURCES})

install(TARGETS bedrock_rga_test DESTINATION bin)
