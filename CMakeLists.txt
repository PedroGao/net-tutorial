cmake_minimum_required(VERSION 3.16)
project(net_tutorial C CXX)

set(CMAKE_C_STANDARD 11)

# add_library(lib ${src}/lib)  
# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)
# target_link_libraries(lib PUBLIC Threads::Threads)

include_directories(lib/)

add_subdirectory(tcp/)
add_subdirectory(udp/)
add_subdirectory(shutdown/)
add_subdirectory(keep_alive/)
add_subdirectory(addr_reuse/)
add_subdirectory(mid_homework/)
add_subdirectory(select/)
add_subdirectory(pool/)
add_subdirectory(epoll/)
add_subdirectory(process_mode/)
add_subdirectory(thread_mode/)