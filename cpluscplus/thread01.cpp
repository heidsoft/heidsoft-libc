#include <iostream>
#include <thread>

// 任务1：计算从1加到100的和
void task1() {
    int sum = 0;
    for (int i = 1; i <= 100; ++i) {
        sum += i;
    }
    std::cout << "Sum of numbers from 1 to 100: " << sum << std::endl;
}

// 任务2：计算5的阶乘
void task2() {
    int factorial = 1;
    for (int i = 1; i <= 5; ++i) {
        factorial *= i;
    }
    std::cout << "Factorial of 5: " << factorial << std::endl;
}

// g++ -o thread01 thread01.cpp -lpthread
int main() {
    // 创建两个线程来执行任务1和任务2
    std::thread thread1(task1);
    std::thread thread2(task2);
    
    // 等待两个线程执行完毕
    thread1.join();
    thread2.join();

    return 0;
}
