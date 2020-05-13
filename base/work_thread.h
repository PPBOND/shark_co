#include "base_thread.h"
#include "coroutine.h"
#include "nolock_buffer.h"
#include "rector_epoll.h"
#include<list>


class work_thread_t
{
public:
    work_thread_t(const std::string& name);
    work_thread_t();
    ~work_thread_t();
    bool addtask(coroutine_t* co);
    bool addtask(func func, void *arg, const std::string& name);
    void thread_start();
    void thread_run();
    void stop();
    bool is_stop();
    void join();
    size_t load();
    static work_thread_t*& get_current_thread();
    int update_event(int fd, int events);
    int remove_event(int fd);
    
private:
    bool m_start;
    epoller_t m_rector;
    thread_t m_thread;
    size_t m_runable;
    coroutine_t* main_coroutine;
    coroutine_t* m_poll_coroutine;
    ring_buffer_t<coroutine_t*> m_coroutine_vers;
public:
    co_env_t* m_env;

    
    
};