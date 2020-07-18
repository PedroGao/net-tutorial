add_executable(poll_react server.c ../lib/common.h ../lib/acceptor.h ../lib/acceptor.c)

TARGET_LINK_LIBRARIES(poll_react pthread)