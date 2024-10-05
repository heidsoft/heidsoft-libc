#include <iostream>

void func(int* ptr) {
    std::cout << "Pointer version" << std::endl;
}

void func(int) {
    std::cout << "Non-pointer version" << std::endl;
}

int main() {
    func(nullptr);

    return 0;
}
