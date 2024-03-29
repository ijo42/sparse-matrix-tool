#include "main.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    srand(time(0));

    auto matrix = generateRnd(3, 4);

    deepDelete(matrix);
    printMatrix(matrix);
    //saveToFile(R"(..\examples\ex0.txt)", matrix);

    return 0;
}
