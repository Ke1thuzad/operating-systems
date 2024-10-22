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
