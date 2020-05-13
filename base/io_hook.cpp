#include "io_hook.h"
#include "work_thread.h"
#include <stdio.h>
#include "coroutine.h"


static __thread bool g_hook_enabled = false;


bool is_hook_enable(){
    return g_hook_enabled;
}
void set_hook_enabled(bool flag){
    g_hook_enabled = flag;
}




extern "C" int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	if(is_hook_enable()){
        auto current_thread = work_thread_t::get_current_thread();
        current_thread->update_event(sockfd, EPOLLIN);

        coroutine_t::get_current_co()->m_status = co_status::waiting;
        coroutine_t::get_current_co()->co_yield();
    }
    int fd = g_sys_accept_func(sockfd, addr, addrlen);
    return fd;
}



extern "C" ssize_t read(int fd, void *buf, size_t count)
{	

    if(is_hook_enable()){
        auto current_thread = work_thread_t::get_current_thread();
        current_thread->update_event(fd, EPOLLIN);

        coroutine_t::get_current_co()->m_status = co_status::waiting;
        coroutine_t::get_current_co()->co_yield();
    }
    
    int ret = g_sys_read_func(fd, buf, count);
	
    return ret;
}


extern "C" ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	size_t wrotelen =0;
	int retlen = g_sys_write_func(fildes, (const char*)buf+ wrotelen, nbyte -wrotelen );
	if(retlen == 0)
		return  retlen;
	
	if(retlen >0)
	{
		wrotelen += retlen;
	}

	while(wrotelen < nbyte)
	{
        if(is_hook_enable()){
            auto current_thread = work_thread_t::get_current_thread();
            current_thread->update_event(fildes, EPOLLOUT);

            coroutine_t::get_current_co()->m_status = co_status::waiting;
            coroutine_t::get_current_co()->co_yield();
        }
		retlen = g_sys_write_func(fildes, (const char*)buf + wrotelen, nbyte -wrotelen );
		
		if(retlen <=0)
			break;
		
		wrotelen += retlen;	
	}

	if(retlen<=0 &&wrotelen ==0 )
		return retlen;

	return wrotelen;
}






ssize_t send(int socket, const void *buffer, size_t length, int flags)
{
    size_t wrotelen = 0;
	ssize_t writeret = g_sys_send_func( socket,buffer,length,flags );
	if (writeret == 0)
	{
		return writeret;
	}

	if( writeret > 0 )
	{
		wrotelen += writeret;	
	}
	while( wrotelen < length )
	{

	    if(is_hook_enable()){
            auto current_thread = work_thread_t::get_current_thread();
            current_thread->update_event(socket, EPOLLOUT);

            coroutine_t::get_current_co()->m_status = co_status::waiting;
            coroutine_t::get_current_co()->co_yield();
        }
		writeret = g_sys_send_func( socket,(const char*)buffer + wrotelen,length - wrotelen,flags );	
		
        if( writeret <= 0 )
		{
			break;
		}
		wrotelen += writeret ;
	}
	if (writeret <= 0 && wrotelen == 0)
	{
		return writeret;
	}

	return wrotelen;
}



ssize_t recv( int socket, void *buffer, size_t length, int flags )
{
     if(is_hook_enable()){
        auto current_thread = work_thread_t::get_current_thread();
        current_thread->update_event(socket, EPOLLIN);

        coroutine_t::get_current_co()->m_status = co_status::waiting;
        coroutine_t::get_current_co()->co_yield();
    }
	ssize_t readret = g_sys_recv_func(socket,buffer,length,flags );
	return readret;	
}
