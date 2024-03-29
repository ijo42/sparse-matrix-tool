#include "main.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    srand(time(0));

    auto matrix1 = generateRnd(4, 4);
    auto matrix2 = generateRnd(4, 4);
    auto matrix3 = generateRnd(4, 4);


    // deepDelete(matrix);
    printMatrix(matrix1);
    printMatrix(matrix2);
    printMatrix(matrix3);
    auto matrix = add(matrix1,matrix2);
    printMatrix(matrix);
    auto matrix0 = sub(matrix1,matrix3);
    printMatrix(matrix0);
    //saveToFile(R"(..\examples\ex0.txt)", matrix);

    return 0;
}
