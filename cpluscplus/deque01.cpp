#include <deque>
#include <iostream>

int main() {
    std::deque<int> d;
    // 在尾部插入元素
    d.push_back(1);
    d.push_back(2);
    // 在头部插入元素
    d.push_front(3);
    d.push_front(4);
    // 输出队列的所有元素
    for(int i = 0; i < d.size(); i++)
        std::cout << d[i] << ' ';
    std::cout << '\n';
    // 删除头部和尾部元素
    d.pop_front();
    d.pop_back();
    // 输出队列的所有元素
    for(int i = 0; i < d.size(); i++)
        std::cout << d[i] << ' ';
    return 0;
}