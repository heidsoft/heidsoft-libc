#指向函数的指针

- 函数指针是指向函数而非指向对象的指针
- 函数类型由其返回类型及其形参确定，而与函数名无关

    bool (*pf)(const string &, const string &)
    这个语句将pf声明未指向函数的指针，它所指向的函数带有两个const string& 类型的形参和bool 类型的返回值

    如果声明是这样，那么将返回bool *
    bool *pf(const string &, string &);
    所以说*pf两侧的括号是必须的

##用typedef 简化函数指针定义，使用typedef为指针类型定义同义词，将函数指针使用大大简化
    typedef bool (*cmpFcn)(const string & , const string &);

##指向函数的指针如何初始化和赋值
    bool lengthCompare(const string & , const string &);

    bool (*)(const string & , const string &)

    可以使用函数名对函数指针做初始化或赋值

    cmpFcn pf1=0; //ok 空指针
    cmpFcn pf2= lengthCompare; //ok
    pf1 = lengthCompare; //ok
    pf2 = pf1; //ok

    此时，直接引用函数名等效于在函数名上应用取地址操作符
    形如：
    cmpFcn pf1 = lengthCompare;
    cmpFcn pf2 = &lengthCompare;
    
##函数指针只能通过同类型的函数和函数指针或0值常量表达式进行初始化和赋值。

##将函指针初始为0，则该指不指任函数。

##指向不类的函指，不存转
    string::size_type subLength(const string& , const string&);

    bool cstringCompare(char * ,char *);

    cmpFcn pf;

    pf = subLength;//error
    pf = cstringCompare; //error
    pf = lengthCompare;//ok

##通过指针调用函数
    cmpFcn pf= lengthCompare;
    lengthCompare("hi","bye");//直接调用
    pf("hi","bye"); //间接调用，通过指针函数
    (*pf)("hi","bye");//间接调用

##如果指向函数的指针没有被初始化，或者具有0值，则该指针不能再函数中调用，只有初始化的指针，或者被赋值指向某个函数，
    才可以安全使用调用函数

##函数指针形参
    函数的形参可以是指向函数的指针，这种形参可以用以下两种方式写
    void userBiger(const string & , const string &, bool(const string& ,string &));

    或者

    void userBiger(const string & , const string &, bool(*)(const string& ,string &));
##返回函数指针
    函数可以返回指向函数的指针

    int (*ff(int)(int*, int));


    阅读函数指针声明的最佳方法是从声明的名字开始 由 里到外 理解

    ff(int) //将ff声明为一个函数，他带有int类型的形参

    该函数返回
    int (*)(int *, int);
    它是一个指向函数的指针，所以指向的函数返回int 型并带有两个形参

    使用typedef 可以使该函数清晰简化
    typedef int func(int *, int) 

    void f1(func);//ok
    func f2(int);//error
    func *f3(int); //ok
##指向重载函数的指针
    c++允许使用函数指针指向重载函数，但指针的类型必须与重载函数的一个版本精确匹配
    例如
    extern void ff(vecter <double>);
    exern void ff(unsigned int);

    重载
    void (*pf2)(int) =&ff;//error
    double (*pf3)(vecter<double>);
    pf3=&ff;//error
