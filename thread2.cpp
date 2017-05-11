#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

class LofFile{
    public:
        LofFile(){
            f.open("log.txt");
        }

        void shared_print(std::string id,int value){
            std::lock(m_mutex,m_mutex2);
            std::lock_guard<std::mutex> locker(m_mutex,std::adopt_lock);
            std::lock_guard<std::mutex> locker2(m_mutex2,std::adopt_lock);
            std::cout << "from" << id << ":" << value << std::endl;
        }

        void shared_print2(std::string id,int value){
            std::lock(m_mutex,m_mutex2);
            std::lock_guard<std::mutex> locker2(m_mutex2,std::adopt_lock);      //lock_guard是为了防止语句执行中出现异常，锁不被释放。这样做可以保证在退出代码块后解锁
            std::lock_guard<std::mutex> locker(m_mutex,std::adopt_lock);
            std::cout << "from" << id << ":" << value << std::endl;
        }

    protected:
    private:
        std::mutex m_mutex;     //使用信号量来解决资源竞争
        std::mutex m_mutex2;
        std::ofstream f;

};

void function_1(LofFile& log){
    for(int i = 0; i > -100; i--){
        log.shared_print("From t1:",i);
    }
};

int main(){
    LofFile log;
    std::thread t1(function_1,std::ref(log));//线程开始运行
    for(int i = 0; i < 100; i++){
        log.shared_print2("From main",i);
    }

    t1.join();
    return 0;
}
