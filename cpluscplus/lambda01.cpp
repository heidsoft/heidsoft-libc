#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 5, 2, 4, 3};

    // 使用lambda表达式进行排序
    std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a < b;
    });

    // 输出排序后的结果
    for (int number : numbers) {
        std::cout << number << " ";
    }
    std::cout << std::endl;

    // 使用lambda表达式进行筛选
    std::vector<int> evenNumbers;
    std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(evenNumbers), [](int number) {
        return number % 2 == 0;
    });

    // 输出筛选后的结果
    for (int number : evenNumbers) {
        std::cout << number << " ";
    }
    std::cout << std::endl;

    return 0;
}
