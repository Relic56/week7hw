cmake_minimum_required(VERSION 2.8)
project(week7)
add_executable(week7 main.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(week7 ${OpenCV_LIBS})



