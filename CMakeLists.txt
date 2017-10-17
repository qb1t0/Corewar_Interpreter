cmake_minimum_required(VERSION 3.6)
project(untitled1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES asm.h main.c head.c label.c cmds.c output.c)
add_executable(untitled1 ${SOURCE_FILES})
target_link_libraries(untitled1 ${CMAKE_SOURCE_DIR}/libft/libft.a)