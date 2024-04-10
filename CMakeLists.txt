cmake_minimum_required(VERSION 3.27)
project(sparse_matrix_consoleline)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_library(SparseMatrix operations.cpp io.cpp util.cpp) #����� .lib �� ��������
#add_library(SparseMatrix SHARED operations.cpp io.cpp util.cpp) ���� ���� �� .dll �����
#���� ��� �� �� �������� ������� ���� ���� � ����� ��������� �����
add_executable(sparse_matrix_consoleline main.cpp
)

target_link_libraries(sparse_matrix_consoleline SparseMatrix)
