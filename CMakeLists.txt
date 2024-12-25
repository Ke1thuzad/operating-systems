cmake_minimum_required(VERSION 3.20)
project(operational_systems C)

set(CMAKE_C_STANDARD 11)

add_executable(lab1 lab1/main.c lab1/main.h
        utility/array.c
        utility/array.h
        utility/error_handler.c
        utility/error_handler.h
        utility/universal.h
        utility/universal.c)

add_executable(child lab1/child.c lab1/main.h
        utility/array.c
        utility/array.h
        utility/error_handler.c
        utility/error_handler.h
        utility/universal.h
        utility/universal.c)

add_executable(lab2 lab2/main.c lab2/main.h
        utility/error_handler.c
        utility/error_handler.h)

add_executable(lab3parent lab3/main.c lab3/main.h
        utility/array.c
        utility/array.h
        utility/error_handler.c
        utility/error_handler.h
        utility/universal.h
        utility/universal.c)

add_executable(lab3child lab3/child.c lab3/main.h
        utility/error_handler.c
        utility/error_handler.h
        utility/universal.h
        utility/universal.c)


add_library(buddy_alloc SHARED lab4/allocators/buddy.c lab4/allocators/buddy.h)
add_library(mkk_alloc SHARED lab4/allocators/mkk.c lab4/allocators/mkk.h)
add_library(emergency SHARED lab4/allocators/emergency.c lab4/allocators/emergency.h)

add_executable(lab4 lab4/main.c lab4/main.h)

target_link_libraries(lab4 PRIVATE buddy_alloc
        PRIVATE mkk_alloc
        PRIVATE emergency)
