#pragma once
#include <functional>
#include <iostream>
#include <ucontext.h>
#include <cstring>
#include <string>

#define stack_size 8096
enum class co_status { init = 1, ready, running, sleeping, waiting, exit};
using func = std::function<void* (void*)>;


class co_env_t;

class coroutine_t
{
public:
    coroutine_t(co_env_t* env);
    coroutine_t(func cb, void *arg, const std::string& name, co_env_t* env);
    ~coroutine_t();
    void co_resume();
    void co_yield();
    static coroutine_t*&  get_current_co();
    static void func_entry(coroutine_t* self);
    std::string name();
    co_status   m_status;


private:
    co_env_t*   m_co_env;
    size_t      m_routine_id;
    const std::string m_name;
    void*       m_arg;
    void*       m_ret;
    void*       m_stack;
    size_t      m_stack_size;
    func        m_cb;
    
    ucontext_t  m_context;
};



class co_env_t {    
public:
    co_env_t(){
        call_index = 0;
    }
    coroutine_t* call_stack[128];
    size_t  call_index = 0;
};


