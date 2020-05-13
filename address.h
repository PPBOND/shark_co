#pragma once

#include <netinet/in.h>
#include <string>
class address_t {
public:
	address_t(std::string ip, in_port_t port);
	explicit address_t(in_port_t port = 0);
	explicit address_t(const struct sockaddr_in& addr);

	
	std::string to_string() const;
	const struct sockaddr* getsock_addr() const;
	struct sockaddr* getsock_addr();
    socklen_t getaddr_length(){
        return sizeof(m_addr);
    }

private:
	struct sockaddr_in m_addr;
};