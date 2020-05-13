#pragma once
#include<vector>
#include<string>
#include "Log.h"
#include "coroutine.h"
#include "work_thread.h"
#include <mutex>  
#include <condition_variable>


enum class load_type { robin = 1, least};

class scheduler_t{
public:
    

    scheduler_t(int thread_num = 1):m_thread_num(thread_num),main_thread("main_thread")
    ,m_start(false),
    m_asyn_sched_thread(std::bind(&scheduler_t::start, this),"schedule_thread"){
    }
    ~scheduler_t(){  stop(); }
    
    void start(){
        
        for(int i =0 ;i < m_thread_num; ++i){
            work_threads_vers.push_back(new work_thread_t());
        }

        for(auto& work_thread: work_threads_vers ){
            work_thread->thread_start();
        }

        
        std::unique_lock <std::mutex> lck(mtx);
        m_start = true;
        m_start_cv.notify_one();    
        lck.unlock();

        main_thread.thread_run();
    }

    void async_start(){
        m_asyn_sched_thread.start();

        std::unique_lock < std::mutex > lck(mtx);
        while(!m_start){
            m_start_cv.wait(lck);
        }
    }


    void stop(){
        if(!m_start){
            return ;
        }
            
        for(auto& work_thread: work_threads_vers ){
            work_thread->stop();
        }

        join_thread();
        
        for(auto& work_thread: work_threads_vers ){
            delete work_thread;
        }

        std::unique_lock <std::mutex> lck(mtx);
        m_start = false;
        m_stop_cv.notify_one();
    }


    void join_thread(){
        for(auto& work_thread: work_threads_vers ){
            work_thread->join();
        }

    }
    bool addtask(func func, void *arg,const std::string& name){
        bool ret = false;
        work_thread_t* choosed_thread = choose_thread_with_balance(load_type::robin);
        ret = choosed_thread->addtask(func, arg, name);
        if(ret == false){
            choosed_thread = choose_thread_with_balance(load_type::least);
            ret = choosed_thread->addtask(func, arg, name);
        }

        return ret;
    }

    work_thread_t* choose_thread_with_balance(load_type type = load_type::robin){
        //轮询
        LOG_DEBUG <<work_threads_vers.size();
        if(type == load_type::robin){
            static size_t index = 0;
            return work_threads_vers[((index++) % work_threads_vers.size())];
        }

        work_thread_t* min_load_thread = nullptr;
        for(auto & single_thread:work_threads_vers ){
            if(min_load_thread == nullptr ||
                min_load_thread->load() < single_thread->load())
                min_load_thread = single_thread;
        }
        return min_load_thread;
    }

    void wait_all(){
        std::unique_lock < std::mutex > lck(mtx);
        while(m_start){
            m_stop_cv.wait(lck);
        }
        
    }

private:
    bool m_start;
    int m_thread_num;
    std::vector<work_thread_t*> work_threads_vers;
    work_thread_t main_thread;
    thread_t m_asyn_sched_thread;
    std::mutex mtx;
    std::condition_variable m_stop_cv;
    std::condition_variable m_start_cv;
};