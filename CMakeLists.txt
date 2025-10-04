cmake_minimum_required(VERSION 4.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME echo)
project(${PROJECT_NAME})

add_executable("server" eserver.cpp)
add_executable("client" eclient.cpp)
