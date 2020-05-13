#include "coroutine.h"
#include "work_thread.h"
static size_t local_co_id = 0;


coroutine_t*& coroutine_t::get_current_co(){
    work_thread_t*& cur_current= work_thread_t::get_current_thread();
    return cur_current->m_env->call_stack[cur_current->m_env->call_index -1];
}


  
coroutine_t::coroutine_t(co_env_t* env):m_co_env(env),m_name("main_coroutine"){
    m_routine_id = local_co_id++;
    getcontext(&m_context);
    
    if(m_co_env->call_stack[m_co_env->call_index] == nullptr && m_co_env->call_index == 0){
        m_co_env->call_stack[m_co_env->call_index++] = this;
    }
    else{
        //error
    }
    

   
}
coroutine_t::coroutine_t(func cb,void* arg, const std::string& name, co_env_t* env):m_cb(cb),m_arg(arg),m_name(name),
m_co_env(env)
{
    m_routine_id = local_co_id++;
    m_stack = malloc(stack_size);
    m_stack_size = stack_size;
    m_status = co_status::ready;
    m_arg = arg;

    getcontext(&m_context);
    m_context.uc_stack.ss_sp    = m_stack;
    m_context.uc_stack.ss_size  = m_stack_size;
    m_context.uc_stack.ss_flags = 0;
    m_context.uc_link = NULL; 

    makecontext(&m_context, (void (*)())coroutine_t::func_entry, 1, this);

}

coroutine_t::~coroutine_t(){
    if(m_stack != nullptr){
        free(m_stack);
    }
}


void coroutine_t::co_resume(){
    coroutine_t* prev = get_current_co();
    m_co_env->call_stack[m_co_env->call_index++] = this;
    swapcontext(&prev->m_context, &this->m_context);
}

void coroutine_t::co_yield(){
    coroutine_t* next = m_co_env->call_stack[m_co_env->call_index -2];
    m_co_env->call_index--;
    swapcontext(&this->m_context, &next->m_context);
}


void coroutine_t::func_entry(coroutine_t* self){
    if(self->m_cb){
        self->m_ret = self->m_cb(self->m_arg);
    }
    self->m_status  = co_status::exit;
    self->co_yield();

}

std::string coroutine_t::name()
{
    return m_name;
}


