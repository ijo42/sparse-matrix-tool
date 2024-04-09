cmake_minimum_required(VERSION 3.27)
project(sparse_matrix_consoleline)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_library(SparseMatrix operations.cpp io.cpp util.cpp) #здесь .lib он статичен
#add_library(SparseMatrix SHARED operations.cpp io.cpp util.cpp) если кому то .dll нужен
#если что то не работает сносите весь билд и потом собирайте снова
add_executable(sparse_matrix_consoleline main.cpp
)

target_link_libraries(sparse_matrix_consoleline SparseMatrix)