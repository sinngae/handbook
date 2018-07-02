/*************************************************************************
	> File Name: inherit.cpp
	> Author: renzq
	> Mail: zqren@foxmail.com
	> Created Time: 2018年06月28日 星期四 13时43分39秒
 ************************************************************************/

#include <iostream>

using namespace std;

class base
{
public:
    virtual void func0(){ cout << "base func0" << endl; }
    virtual void funca()=0;
    void funcx() { cout << "base funcx" << endl; }

};

void base::funca() {cout << "base funca" << endl;}

void nofunc();

class derived : public base
{
public:
    void func0() { cout << "derived func0" << endl; }
    virtual void func_() { cout << "deriverd func000" << endl; }
    void funca(){ cout << "derived funca" << endl; }
    void funcx() { cout << "derived funcx" << endl; }
    void func0(int a) { cout << "derived func0 int" << endl; }
};

void func(base &b)
{
    b.funca();
}

int main(int argc, char *argv[])
{
    derived d;
    d.func0();
    d.derived::func_();
    d.funca();
    //d.base.funca();

    func(d);

    base &b = d;
    b.funca();
    b.func0();
    b.funcx();
//    b.func0(1);
    b.base::funca();
    
    base *p = &d;
    p->base::funca();

    return 0;
}
