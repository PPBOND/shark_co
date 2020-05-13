#pragma once


class address_t;
class socket_t{

public:
    socket_t(int fd);
    ~socket_t();
    int read(char* buf ,size_t length);
    int write(char* buf,size_t length);
    void bind(const address_t& local_addr);
    void listen();
    int accept(address_t& client_addr);
    int connect(address_t& server_addr);
    void set_no_block();
    void close();
    int  fd();

private:
    int m_fd;
};