cmake_minimum_required(VERSION 3.27)
project(sparse_matrix_consoleline)

set(CMAKE_CXX_STANDARD 23)

add_executable(sparse_matrix_consoleline main.cpp
        main.h
        shared.h
        io.cpp
        io.h
        operations.cpp
        operations.h
        util.cpp
        util.h
)
