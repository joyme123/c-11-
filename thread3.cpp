#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

class LofFile{
    public:
        LofFile(){
            //f.open("log.txt");
        }

        void shared_print(std::string id,int value){

            std::call_once(m_flag,[&](){f.open("log.txt");});               //使用call_once保证只调用一次
            std::unique_lock<std::mutex> locker(m_mutex,std::defer_lock);   
            std::cout << "from" << id << ":" << value << std::endl;
        }


    protected:
    private:
        std::mutex m_mutex;
        std::once_flag m_flag;
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
        log.shared_print("From main",i);
    }

    t1.join();
    return 0;
}
