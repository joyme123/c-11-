#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include <deque>

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;       //类似于观察者模式，

void function_1(){
    int count = 10;
    while(count > 0){
        std::unique_lock<std::mutex> locker(mu);    //unique_lock和lock_guard的不同在于unique可被多次枷锁解锁
        q.push_front(count);
        locker.unlock();
        cond.notify_all();      //发出通知
        std::this_thread::sleep_for(std::chrono::seconds(1));   //线程休眠
        count--;
    }
}

void function_2(){
    int data = 0;
    while(data != 1){
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker,[](){return !q.empty();});     //等待，直到收到通知才运行
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1:" << data << std::endl;


    }
}


int main(){
    std::thread t1(function_1);//线程开始运行
    std::thread t2(function_2);
    t1.join();
    t2.join();
    return 0;
}
