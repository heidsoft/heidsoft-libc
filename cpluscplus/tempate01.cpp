#include <iostream>

// 定义一个通用函数模板
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// g++ -o tempate01 tempate01.cpp
int main() {
    int x = 10, y = 20;
    std::cout << "较大的数是：" << maximum(x, y) << std::endl; // 输出 20

    double a = 3.14, b = 2.71;
    std::cout << "较大的数是：" << maximum(a, b) << std::endl; // 输出 3.14

    return 0;
}
