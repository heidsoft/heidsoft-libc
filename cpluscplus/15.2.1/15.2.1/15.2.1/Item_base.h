//  c++ 基类练习
//  基类通常应该将派生类需要定义的任意函数定义为虚函数
//  Item_base.h
//  15.2.1
//
//  Created by Jake liu on 2022/2/13.
//

#ifndef Item_base_h
#define Item_base_h

class Item_base {
public:
    Item_base(const std::string &book = "",double sales_price = 0.0):
                                        isbn(book),price(sales_price){}
    
    std::string book() const {return isbn;}
    
    virtual double net_price(std::size_t n) const{
        return n*price;
    }
    //虚析构函数
    //virtual的作用是启动绑定
    //除了构造函数之外，任意static成员函数都是可以虚函数
    //virtual 只在类内部的成员函数声明周出现，不能在类定义体外部出现的函数定义上
    virtual ~Item_base(){ }
private:
    std::string isbn;
protected:
    double price;
};

#endif /* Item_base_h */
