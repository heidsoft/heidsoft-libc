#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::vector<int>&& rvalueRef = std::move(numbers);

    std::cout << "Moved vector size: " << numbers.size() << std::endl;

    return 0;
}
