#include "rector_epoll.h"
#include "work_thread.h"


epoller_t::epoller_t(work_thread_t* thread):m_thread(thread){
    m_epoll_fd = epoll_create(1);
}

epoller_t::~epoller_t(){
    for(auto & del_co_pair :m_fd_to_couroutine){
        delete del_co_pair.second;
    }
}

int epoller_t::update_event(int fd, int events){
    
    int fd_ops = EPOLL_CTL_MOD;
    if(m_fd_to_couroutine[fd] == NULL){
        m_fd_to_couroutine[fd] = coroutine_t::get_current_co();
        fd_ops = EPOLL_CTL_ADD;
    }

    struct epoll_event epoll_ev;
    epoll_ev.events = events;
    epoll_ev.data.fd = fd;
    return epoll_ctl(m_epoll_fd, fd_ops, fd, &epoll_ev);

}

int epoller_t::remove_event(int fd){
    m_fd_to_couroutine.erase(fd);
    return epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL , fd, NULL);
}


void* epoller_t::handle_event(void *arg){

    int timeout = reinterpret_cast<int>(arg);
    while(!m_thread->is_stop()){
        int active_num = epoll_wait(m_epoll_fd, m_active_ev, 100,timeout);
       
        for(int i =0 ;i < active_num; ++i){

            int active_fd = m_active_ev[i].data.fd;
            coroutine_t* runable_co = m_fd_to_couroutine[active_fd];
            runable_co->m_status = co_status::ready;
            remove_event(active_fd);
            m_thread->addtask(runable_co);

        } 
        coroutine_t::get_current_co()->co_yield();
    }

    return nullptr;
}