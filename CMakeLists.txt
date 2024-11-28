cmake_minimum_required(VERSION 3.5.0)
project(ATSP VERSION 0.1.0 LANGUAGES C CXX)

# Add the include direcotry for header file
include_directories(headers)

add_executable(ATSP src/main.cpp src/Algorithm.cpp src/Node.cpp)

