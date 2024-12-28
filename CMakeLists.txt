cmake_minimum_required(VERSION 3.10)
project(rubiks-cube-solver)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(solver main.cpp)

# Link OpenCV libraries
target_link_libraries(solver ${OpenCV_LIBS})
