#include "server.h"
/*


class tcp_server_t{
public:
    using conn_back_func = std::function<void (tcp_socket_t*)>;
    tcp_server_t(const address_t& listen_addr, scheduler_t* sched);
    void start();
    void set_conn_callback(conn_back_func handler);
private:
    void start_accept();
    address_t m_listen_addr;
    socket_t* m_listen_sock;
    scheduler_t* m_sched;
    conn_back_func m_conn_handler;


};
*/

void* default_handler(tcp_socket_t* conn){
    std::cout<<conn->get_addr().to_string()<<std::endl;
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
    m_sched->addtask(std::bind(start_accept, this,_1), NULL, "accept");
}


void* tcp_server_t::start_accept(void *arg){

    while(true){
        address_t peer_addr;
        int connfd = m_listen_sock->accept(peer_addr);
        socket_t* sock = new socket_t(connfd);
        sock->set_no_block();
        m_sched->addtask(std::bind(conn_back_func, this, new tcp_socket_t(sock,peer_addr)), NULL, "accept");


    }
}