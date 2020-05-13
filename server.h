#include "tcp_socket.h"
#include "scheduler.h"

class tcp_server_t{
public:
    using conn_back_func = std::function<void* (tcp_socket_t*)>;
    tcp_server_t(const address_t& listen_addr, scheduler_t* sched);
    void start();
    void set_conn_callback(conn_back_func handler);
private:
    void* start_accept();
    address_t m_listen_addr;
    socket_t* m_listen_sock;
    scheduler_t* m_sched;
    conn_back_func m_conn_handler;


};