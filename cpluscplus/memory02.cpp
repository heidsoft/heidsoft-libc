#include <memory>
#include <iostream>
int main() {
    std::shared_ptr<int> ptr1(new int(5));
    std::shared_ptr<int> ptr2 = ptr1;

    // 使用ptr1和ptr2指针访问动态分配的内存
    std::cout << *ptr1 << std::endl;
    std::cout << *ptr2 << std::endl;

    // 不需要手动释放内存，shared_ptr会在所有共享指针都不再引用内存时自动释放内存
    return 0;
}
