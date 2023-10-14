#include <iostream>
#include <valarray>

int main() {
    std::valarray<int> val1 = {1, 2, 3, 4, 5}; // 初始化
    std::valarray<int> val2 = {5, 4, 3, 2, 1};

    // 元素级别的运算
    std::valarray<int> result = val1 + val2;
    // 使用常规的for循环
    for (std::size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    // 使用统计函数
    std::cout << "Sum of val1: " << val1.sum() << std::endl;
    std::cout << "Max of val1: " << val1.max() << std::endl;

    return 0;
}