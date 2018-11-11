cmake_minimum_required(VERSION 3.9)
project(code)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )
set(SOURCE_FILES main.cpp)
add_executable(OpenCVTest ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( OpenCVTest ${OpenCV_LIBS} )
