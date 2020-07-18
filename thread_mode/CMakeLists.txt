add_executable(thread_mode server.c ../lib/common.h)

TARGET_LINK_LIBRARIES(thread_mode pthread)