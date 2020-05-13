#pragma once
#include <dlfcn.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h> 
#include <sys/socket.h>



typedef int (*socket_pfn_t)(int domain, int type, int protocol);
typedef ssize_t (*read_pfn_t)(int fildes, void *buf, size_t nbyte);
typedef int (*accept_pfn_t)(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
typedef ssize_t (*write_pfn_t)(int fildes, const void *buf, size_t nbyte);
typedef size_t (*send_pfn_t)(int socket, const void *buffer, size_t length, int flags);
typedef ssize_t (*recv_pfn_t)(int socket, void *buffer, size_t length, int flags);
typedef unsigned int (*sleep_pfn_t)(unsigned int seconds);

typedef ssize_t (*sendto_pfn_t)(int socket, const void *message, size_t length,
	                 int flags, const struct sockaddr *dest_addr,
					               socklen_t dest_len);

typedef ssize_t (*recvfrom_pfn_t)(int socket, void *buffer, size_t length,
	                 int flags, struct sockaddr *address,
					               socklen_t *address_len);
typedef int (*close_pfn_t)(int fd);




static accept_pfn_t g_sys_accept_func 	  = (accept_pfn_t)dlsym(RTLD_NEXT,"accept");
static read_pfn_t g_sys_read_func 		  = (read_pfn_t)dlsym(RTLD_NEXT,"read");
static write_pfn_t g_sys_write_func 	  = (write_pfn_t)dlsym(RTLD_NEXT,"write");
static recv_pfn_t g_sys_recv_func 		  = (recv_pfn_t)dlsym(RTLD_NEXT,"recv");
static send_pfn_t g_sys_send_func 		  = (send_pfn_t)dlsym(RTLD_NEXT,"send");
static sendto_pfn_t g_sys_sendto_func 	  = (sendto_pfn_t)dlsym(RTLD_NEXT,"sendto");
static recvfrom_pfn_t g_sys_recvfrom_func = (recvfrom_pfn_t)dlsym(RTLD_NEXT,"recvfrom");
static sleep_pfn_t g_sys_sleep_func       = (sleep_pfn_t)dlsym(RTLD_NEXT,"sleep");
static socket_pfn_t g_sys_socket_func 	= (socket_pfn_t)dlsym(RTLD_NEXT,"socket");
static close_pfn_t g_sys_close_func 	= (close_pfn_t)dlsym(RTLD_NEXT,"close");


bool is_hook_enable();
void set_hook_enabled(bool flag);


