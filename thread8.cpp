#include <iostream>
#include <thread>
#include <deque>
#include <future>

using namespace std;

int factorial(int N){
    int res;
    for(int i = N; i > 1; i++){
        res *= i;
    }


    cout << "Result is:" << res << endl;

    return res;
}

std::deque<std::packaged_task<int()> > task_q;
std::condition_variable cond;

int main(){
    std::thread t1(factorial,6);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    chrono::steady_clock::time_point tp = chrono::steady_clock::now() + chrono::microseconds(4);
    std::this_thread::sleep_until(tp);

    std::mutex mu;
    std::unique_lock<std::mutex> locker(mu);
    locker.try_lock_for(chrono::milliseconds(3));
    locker.try_lock_until(tp);

    std::condition_variable cond;
    cond.wait_for(locker,chrono:milliseconds(3));
    cond.wait_until(locker,tp);

    std::promise<int> p;
    std::future<int> f = p.get_future();
    f.wait_for(chrono::milliseconds(3));
    f.wait_until(tp);

}
