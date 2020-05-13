#pragma once
#include <map>
#include "coroutine.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>


class work_thread_t;

class epoller_t{
public:
    epoller_t(work_thread_t* thread);
    ~epoller_t();
    int update_event(int fd, int events);
    int remove_event(int fd);
    void* handle_event(void* arg);
private:
    work_thread_t* m_thread;
    std::map<int, coroutine_t*> m_fd_to_couroutine;
    struct epoll_event m_active_ev[100];
    int m_epoll_fd;
};