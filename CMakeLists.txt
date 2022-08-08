cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)

project(bedrock_rga_test)

set(SCHARE_INCLUDE  ${CMAKE_CURRENT_LIST_DIR}/lib/librga/include
                    ${CMAKE_CURRENT_LIST_DIR}/lib/librga/im2d_api
                    ${CMAKE_CURRENT_LIST_DIR}/include
                    ${CMAKE_CURRENT_LIST_DIR}/lib/libstb/include

)

include_directories(${SCHARE_INCLUDE})

set(SOURCES main.cpp
            src/rga_test.cpp
            src/argparse.cpp
)
set(SCHARE_LIBS
    rga
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SCHARE_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
