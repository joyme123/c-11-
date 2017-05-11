#include <iostream>
#include <thread>
#include <future>
#include <deque>
#include <mutex>

using namespace std;

int factorial(int N){
    int res = 1;

    for(int i = N; i > 1; i--){
        res *= i;
    }

    cout << "Result is:" << res << endl;
    return res;
}
std::deque<std::packaged_task<int()> > task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1(){
    std::packaged_task<int()>  t;

    {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker,[](){return !task_q.empty();});
        t = std::move(task_q.front());
    }
    t();
}


int main(){
    std::thread t1(thread_1);
    std::packaged_task<int()> t(std::bind(factorial,6));
    std::future<int> ret = t.get_future();

    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(t));
    }
    cond.notify_one();
    int value = ret.get();
    t1.join();
    cout << value << endl;
    return 0;
}
