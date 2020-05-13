#include "base_socket.h"
#include "address.h"
class tcp_socket_t{

public:
    tcp_socket_t(socket_t* conn_sock,address_t peer_addr);
    int read(char* buf ,size_t length);
    int write(char* buf,size_t length);
    void close();
    const address_t& get_addr() const { return m_peer_addr;}

private:
    socket_t*  m_conn_sock;
    address_t  m_peer_addr;


};