#include "work_thread.h"
#include "Log.h"
#include <functional>
using namespace std::placeholders;

work_thread_t*& work_thread_t::get_current_thread(){
    static __thread work_thread_t* cur_thread = nullptr;
    return cur_thread;
}


work_thread_t::work_thread_t(const std::string& name ="work_thread"):m_thread(std::bind(&work_thread_t::thread_run, this),name),m_rector(this),
m_coroutine_vers(100){
    m_env = new co_env_t();
    main_coroutine   = new coroutine_t(m_env);
    m_poll_coroutine = new coroutine_t(std::bind(&epoller_t::handle_event, &m_rector,_1), reinterpret_cast<void*>(1000),"poll_coroutine", m_env);
    m_runable = 0;
}


work_thread_t::work_thread_t():m_thread(std::bind(&work_thread_t::thread_run, this),"work_thread"),m_rector(this),
m_coroutine_vers(100){
    m_env = new co_env_t();
    main_coroutine   = new coroutine_t(m_env);
    m_poll_coroutine = new coroutine_t(std::bind(&epoller_t::handle_event, &m_rector,_1), reinterpret_cast<void*>(1000),"poll_coroutine", m_env);
    m_runable = 0;
}


bool work_thread_t::addtask(func func, void *arg, const std::string& name){
    bool ret = false;
    coroutine_t* co_task = new coroutine_t(func, arg, name, m_env);
    if(ret = m_coroutine_vers.push_back(co_task))
        m_runable++;
    return ret;
}

bool work_thread_t::addtask(coroutine_t* co){
    bool ret = false;
    if(ret = m_coroutine_vers.push_back(co))
        m_runable++;
    return ret;
}

void work_thread_t::thread_start(){
    m_thread.start();
}
    
work_thread_t::~work_thread_t(){
    if(m_env != nullptr){
        delete m_env;
        delete main_coroutine;
        delete m_poll_coroutine;
    }

    for(int i =0; i< m_coroutine_vers.size(); ++i){
        coroutine_t* del_coroutine = nullptr;
        m_coroutine_vers.pop_front(del_coroutine);
        if(del_coroutine == nullptr)
            continue;
        delete del_coroutine;
    }


}

void work_thread_t::thread_run(){

     m_start = true;
    if(get_current_thread() == nullptr){
        get_current_thread() = this;
    }

    while(!is_stop()){
        
        if(m_coroutine_vers.empty()){
            LOG_DEBUG<<"co_vers is empty()";
            m_poll_coroutine->co_resume();
        }
        else{

            coroutine_t * cur = nullptr;
            m_coroutine_vers.pop_front(cur);
           
            cur->co_resume();
            if(cur->m_status == co_status::exit){
                --m_runable;
                delete cur;
            }
        }   
    }
}



bool work_thread_t::is_stop(){
    return !m_start;
}
void work_thread_t::join(){
    m_thread.join();   
}

size_t work_thread_t::load(){
    return m_runable;
}

void work_thread_t::stop(){
    m_start = false;
}



int work_thread_t::update_event(int fd, int events){
    return m_rector.update_event(fd, events);
}
int work_thread_t::remove_event(int fd){
    return m_rector.remove_event(fd);
}