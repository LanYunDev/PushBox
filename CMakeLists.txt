cmake_minimum_required(VERSION 3.22)
project(PushBox C)

set(CMAKE_C_STANDARD 23)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(PushBox main.c)
target_link_libraries(PushBox ${CURSES_LIBRARY})
add_executable(test test.c)
target_link_libraries(test ${CURSES_LIBRARY})
add_executable(test2 test2.c)
target_link_libraries(test2 ${CURSES_LIBRARY})
