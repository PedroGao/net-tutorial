cmake_minimum_required(VERSION 3.16)
project(net_progressive C)

set(CMAKE_C_STANDARD 11)

include_directories(lib/)

add_subdirectory(basic_tcp)
add_subdirectory(basic_udp)
add_subdirectory(basic_http)
add_subdirectory(basic_select)
add_subdirectory(basic_poll)
