
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "base_socket.h"
#include "address.h"


socket_t::socket_t(int fd):m_fd(fd){}
socket_t::~socket_t(){}
int socket_t::read(char* buf ,size_t length){
    ssize_t n = ::read(m_fd, buf, length);
	if (n < 0) {	
	}
	return n;
}
int socket_t::write(char* buf,size_t length)
{
     ssize_t n = ::write(m_fd, buf, length);
	if (n < 0) {	
	}
	return n;

}
void socket_t::set_no_block(){
	int flags = ::fcntl(m_fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	
	flags = ::fcntl(m_fd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	::fcntl(m_fd, F_SETFD, flags);
	
}
void socket_t::close(){
    ::close(m_fd);
}
int socket_t::fd(){
    return m_fd;
}

void socket_t::bind(const address_t& local_addr){
	::bind(m_fd,local_addr.getsock_addr(),local_addr.getaddr_length());
}
void socket_t::listen(){
	::listen(m_fd, SOMAXCONN);
}
int socket_t::accept(address_t& client_addr){
	socklen_t addrlen = client_addr.getaddr_length();
	int conn_fd = ::accept(m_fd, client_addr.getsock_addr(), &addrlen);
	return conn_fd;
}
int socket_t::connect(address_t& server_addr){
	return -1;
}


