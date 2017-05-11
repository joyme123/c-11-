#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <future>

using namespace std;

int factorial(std::shared_future<int> f){
    int res = 1;

    int n = f.get();
    for (int i = n; i > 1; i--)
        res *= i;

    cout << "Result is:" << res << endl;
    return res;
}

int main(){
    int x;


    std::promise<int> p;        //promise中的future,可以让子线程从主线程中拿到值
    std::future<int> f = p.get_future();
    //可以被拷贝
    std::shared_future<int> sf = f.share();

    //std::thread t1(factorial,4);
    std::future<int> fu = std::async(std::launch::async,factorial,sf);
    std::future<int> fu2 = std::async(std::launch::async,factorial,sf);
    std::future<int> fu3 = std::async(std::launch::async,factorial,sf);

    p.set_value(4);     //给承诺赋值，在子线程中会得到这个值
    x = fu.get();

    cout << x << endl;
    //t1.join();
    return 0;
}
