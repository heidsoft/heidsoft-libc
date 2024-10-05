#include <iostream>
#include <list>
int main() {
    // 创建一个空的list
    std::list<int> myList;
    // 使用push_back()函数在list的末尾添加元素
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
    // 使用push_front()函数在list的开头添加元素
    myList.push_front(0);
    // 使用insert()函数在list的指定位置插入元素
    auto it = myList.begin();
    std::advance(it, 2);
    myList.insert(it, -1);
    // 使用erase()函数删除list的指定位置的元素
    it = myList.begin();
    std::advance(it, 3);
    myList.erase(it);
    // 打印list中的所有元素
    for (const auto &elem : myList) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    return 0;
}