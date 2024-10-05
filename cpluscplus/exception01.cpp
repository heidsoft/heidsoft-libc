#include <iostream>

// g++ -o exception01 exception01.cpp
int main() {
    try {
        int numerator, denominator, result;
        
        std::cout << "Enter the numerator: ";
        std::cin >> numerator;
        
        std::cout << "Enter the denominator: ";
        std::cin >> denominator;
        
        if (denominator == 0) {
            throw std::runtime_error("Division by zero error");
        }
        
        result = numerator / denominator;
        
        std::cout << "Result: " <<result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    return 0;
}