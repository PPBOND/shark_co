#pragma once
#include <sys/syscall.h>
#include <string.h>
#include <pthread.h>
#include <functional>
#include <unistd.h>
class thread_t
{
    using func = std::function<void(void)>;
public:
    thread_t(func cb, const std::string& name);
    void start();
    void join();
    static pid_t thread_tid();
    ~thread_t();
  

private:
    static void* thread_entry(void *arg);
    pthread_t m_tid;
    bool    m_start;
    const std::string& m_name;
    func m_cb;
    void* arg;
    
};