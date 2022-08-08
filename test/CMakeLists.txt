# 参考  external/rknpu2/examples/rknn_common_test/CMakeLists.txt 完成opencv的配置

set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/../lib/opencv/share/OpenCV/)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})




add_executable(opencv_test opencv_test.cpp)

target_link_libraries(opencv_test ${OpenCV_LIBRARIES})