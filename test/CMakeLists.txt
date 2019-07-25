add_executable(timespectests timespecTests.c)
target_link_libraries(timespectests timespec)
add_test(NAME timespectests
    COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:timespectests>)
