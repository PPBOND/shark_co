

#include "base_thread.h"
static __thread pid_t t_tid = 0;

pid_t thread_t::thread_tid() {
	if (t_tid == 0) {
		t_tid = ::syscall(SYS_gettid);
	}
	return t_tid;
}


thread_t::thread_t(func cb, const std::string& name):m_cb(cb),m_name(name){

}


thread_t::~thread_t(){
    
}


void  thread_t::start(){
    m_start = true;
    pthread_create(&m_tid, nullptr, thread_t::thread_entry, this);
}

void* thread_t::thread_entry(void *arg){
    thread_t* thread = (thread_t*)arg;
    thread->m_cb();
    return nullptr;
}





void thread_t::join() {

	//joined_ = true;
	if (int error = pthread_join(m_tid, nullptr)) {
	}
}

