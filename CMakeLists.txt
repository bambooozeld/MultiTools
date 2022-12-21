cmake_minimum_required(VERSION 3.24)
project(MultiTools C)

set(CMAKE_C_STANDARD 11)

add_executable(MultiTools main.c Chat.c Chat.h)
target_link_libraries(MultiTools "-lws2_32")
