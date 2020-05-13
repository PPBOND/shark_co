#include "server.h"
#include "io_hook.h"
#include <functional>
using namespace std::placeholders;

void* default_handler(void* arg){
    tcp_socket_t* conn =(tcp_socket_t*)arg;
    std::cout<<"default_handler has been call"<<std::endl;
    LOG_DEBUG<<"default_handler has been call";
    //std::cout<<conn->get_addr().to_string()<<std::endl;
}
tcp_server_t::tcp_server_t(const address_t& listen_addr, scheduler_t* sched)
:m_listen_addr(listen_addr),
m_conn_handler(default_handler),
m_sched(sched)
{
    int inner_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    m_listen_sock = new socket_t(inner_fd);
    m_listen_sock->set_no_block();
    m_listen_sock->bind(listen_addr);
}

void tcp_server_t::start(){
    m_listen_sock->listen();
    m_sched->addtask(std::bind(&tcp_server_t::start_accept, this, nullptr), NULL, "accept");
}


void* tcp_server_t::start_accept(void *arg){

    LOG_DEBUG<<"start_accept has been call";
   // set_hook_enabled(true);
    while(true){
        
        address_t peer_addr;
        int connfd = m_listen_sock->accept(peer_addr);
        
        socket_t* sock = new socket_t(connfd);
        sock->set_no_block();
        LOG_DEBUG<<"socket_fd=" << sock->fd();
        int ret =m_sched->addtask(m_conn_handler, new tcp_socket_t(sock,peer_addr), "connect");
        LOG_DEBUG<<"ret ="<<ret;
    }

    return nullptr;
}

void tcp_server_t::set_conn_callback(conn_back_func handler){
    m_conn_handler = handler;
}