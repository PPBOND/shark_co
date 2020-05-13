#include "server.h"




int main(){

    limlog::setLogFile("./test_log_file");
    limlog::setLogLevel(limlog::LogLevel::DEBUG);
    limlog::setRollSize(64); // 64MB
    address_t listen_addr(5000);
    int thread_num = 1;
    scheduler_t scheduler(thread_num);
    scheduler.async_start();

    tcp_server_t server(listen_addr, &scheduler);
    server.start();
    scheduler.wait_all();
    return 0;
}