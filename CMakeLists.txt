cmake_minimum_required(VERSION 3.24)
project(ChatApp C)

set(CMAKE_C_STANDARD 11)

add_executable(ChatApp main.c Chat.c Chat.h)