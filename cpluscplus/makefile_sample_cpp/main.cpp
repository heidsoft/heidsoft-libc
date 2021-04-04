#include <iostream>


int main(void){

 std::cout<< "hi" << " there" <<std::endl;

 ((std::cout << "hi" <<" there ")) << std::endl;
 
 //说明IO位操作的优先级别问题
 std::cout << 42 + 10;
 std::cout << (10 < 42);
 //this is error 
 //std::cout << 10 < 42; 
 //移位操作符具有中等优先级，其优先级比算术操作符低，比关系操作符，赋值操作符合条件操作符高
 return 0;
}
