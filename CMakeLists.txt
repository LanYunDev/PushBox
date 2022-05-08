cmake_minimum_required(VERSION 3.22)
project(PushBox C)

set(CMAKE_C_STANDARD 23)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(PushBox PushBox.c)
target_link_libraries(PushBox ${CURSES_LIBRARY})

