#include <iostream>
#include <thread>

void function_1(){
    std::cout << "hello world" << std::endl;
}

int main(){
    std::thread t1(function_1);
    t1.detach();        //分离运行
    if(t1.joinable())
        t1.join();      //加入运行
    return 0;
}
