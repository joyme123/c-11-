#include <iostream>
#include <mutex>
#include <fstream>
#include <future>

using namespace std;

class A{
public:
    void f(int x,char c){}
    int operator()(int N){return 0;}
};

void foo(int x){}

int main(){
    A a;
    std::thread t1(a,6);                //传递a的拷贝给子线程
    std::thread t2(std::ref(a),6);      //传递a的引用
    std::thread t3(std::move(a),6);     //移动
    std::thread t4(A(),6);
    std::thread t5(foo,6);
    std::thread t6([](int x){return x*x;},6);
    std::thread t7(&A::f,a,8,'w');
    std::thread t8(&A::f,&a,8,'w');
    std::async(std::launch::async,a,6);

    return 0;


}
