#include <iostream>
using namespace std;

class Base {
   public:
    virtual void f(float x) {
        cout << "Base::f(float): " << x << endl;
    }
    virtual void f(int x) { // 重载：必须在同一个域中
        cout << "Base::f(int): " << x << endl;
    }
    void g(float x) {
        cout << "Base::g(float): " << x << endl;
    }
    void h(float x) {   // virtual
        cout << "Base::h(float): " << x << endl;
    }
    virtual void i(float x) {
        cout << "Base::i(float): " << x << endl;
    }
};

class Derived : public Base {
   public:
    // 子类函数与基类函数同名，有virtual关键字 -> 运行时多态
    virtual void f(float x) {
        cout << "Derived::f(float): " << x << endl;
    }
    // 子类函数与基类函数同名（参数也相同），无virtual关键字 -> 隐藏
    void g(float x) {
        cout << "Derived::g(float): " << x << endl;
    }
    // 子类函数与基类函数同名（参数不同），有/无virtual关键字 -> 隐藏
    void h(int x) {
        cout << "Derived::h(int): " << x << endl;
    }
    // 子类函数与基类函数同名（参数也相同），有virtual关键字 -> 覆盖（重写）
    virtual void i(float x) {
        cout << "Base::i(float): " << x << endl;
    }
};

int main(){
    Derived derived; // 子类对象
    Base *p_base = &derived; // 基类指针指向子类对象
    Derived *p_derived = &derived; // 子类指针指向子类对象
    
    // Good: 行为仅取决于对象的类型
    p_base->f(3.14);
    p_derived->f(3.14);

    // Bad: 行为取决于指针类型
    p_base->g(3.14);
    p_derived->g(3.14);

    // Bad: 行为取决于指针类型
    p_base->h(3.14);
    p_derived->h(3.14);

    // 覆盖（重写）
    p_base->i(3.14);
    p_derived->i(3.14);

    // 重载
    p_base->f(3);
    p_derived->f(3);

    system("pause");
    return 0;
}