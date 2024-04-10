#include <chrono>
#include "headers/main.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    srand(time(0));

    auto l = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    auto matrix1 = generateRnd(500, 500);
//    auto matrix2 = generateRnd(4, 4);
//    auto matrix3 = generateRnd(4, 4);


    // deepDelete(matrix);
//    printMatrix(matrix1);
//    printMatrix(matrix2);
//    printMatrix(matrix3);
//    auto matrix = add(matrix1,matrix2);
//    printMatrix(matrix);
//    auto matrix0 = sub(matrix1,matrix3);
//    printMatrix(matrix0);
    saveToFile(R"(E:\examples\ex500.dlsm)", matrix1);
//    std::cout << std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() - l;

    return 0;
}
