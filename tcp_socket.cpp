#include "tcp_socket.h"
 tcp_socket_t::tcp_socket_t(socket_t* conn_sock,address_t peer_addr):
 m_conn_sock(conn_sock),m_peer_addr(peer_addr){

 }
int tcp_socket_t::read(char* buf ,size_t length){
    return m_conn_sock->read(buf, length);
}
int tcp_socket_t::write(char* buf,size_t length){
    return m_conn_sock->write(buf, length);
}

void tcp_socket_t::close(){
    return m_conn_sock->close(buf, length);
}
