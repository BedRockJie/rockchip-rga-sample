cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)

project(bedrock_rga_test)
set(OpenCV_DIR /home/gaojie/workspace/rk356x/rk3566/buildroot/output/rockchip_rk3566/build/opencv3-3.4.12/buildroot-build/)
# 这里的大小写也很讲究
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SCHARE_INCLUDE  ${CMAKE_CURRENT_LIST_DIR}/lib/librga/include
                    ${CMAKE_CURRENT_LIST_DIR}/lib/librga/im2d_api
                    ${CMAKE_CURRENT_LIST_DIR}/include
                    ${CMAKE_CURRENT_LIST_DIR}/lib/libstb/include

)

include_directories(${SCHARE_INCLUDE})
link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/opencv2/lib)
set(SOURCES main.cpp
            src/rga_test.cpp
            src/argparse.cpp
)
set(SCHARE_LIBS
    rga
)

add_subdirectory(test)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SCHARE_LIBS} ${OpenCV_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
