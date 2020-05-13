#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#include "address.h"

address_t::address_t(std::string ip, in_port_t port) {
	bzero(&m_addr, sizeof m_addr);

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	int s;
	if ((s = ::inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr) <= 0)) {
		
	}
}

address_t::address_t(in_port_t port) {
	bzero(&m_addr, sizeof m_addr);
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	m_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

}

address_t::address_t(const struct sockaddr_in& addr)
	:m_addr(addr) {

}

	
std::string address_t::to_string() const {
	std::stringstream ss;
	char buf[20];
	const char* ip = ::inet_ntop(AF_INET, &m_addr.sin_addr, buf, sizeof(buf));
	if (ip) {
		ss << ip;
	} else {
		ss << "invalid ip";
	}
	ss << ":" << ntohs(m_addr.sin_port);
	return ss.str();
}

const struct sockaddr* address_t::getsock_addr() const {
	const void* void_ptr = static_cast<const void*>(&m_addr);
	return static_cast<const struct sockaddr*>(void_ptr);
}

struct sockaddr* address_t::getsock_addr() {
	void* void_ptr = static_cast<void*>(&m_addr);
	return static_cast<struct sockaddr*>(void_ptr);
}
