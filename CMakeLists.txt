cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(socket_example)

add_subdirectory(client)
add_subdirectory(server)

