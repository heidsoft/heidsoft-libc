#include <iostream>

template<typename T, typename U>
class Pair {
private:
    T first;
    U second;
public:
    Pair(T f, U s) : first(f), second(s) {}

    T getFirst() {
        return first;
    }

    U getSecond() {
        return second;
    }
};

template<typename T>
class Container {
private:
    T value;
public:
    Container(T v) : value(v) {}

    T getValue() {
        return value;
    }
};

int main() {
    Pair<int, Container<double>> myPair(5, Container<double>(3.14));

    std::cout << "First: " << myPair.getFirst() << std::endl;
    std::cout << "Second: " << myPair.getSecond().getValue() << std::endl;

    return 0;
}
