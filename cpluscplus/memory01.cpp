#include <memory>
#include <iostream>
int main() {
    std::unique_ptr<int> ptr(new int(5));

    // 使用ptr指针访问动态分配的内存
    std::cout << *ptr << std::endl;

    // 不需要手动释放内存，unique_ptr会在其作用域结束时自动释放内存
    return 0;
}
