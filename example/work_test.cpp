#include "scheduler.h"
#include "Log.h"
scheduler_t sched(3);
void* cb1(void *arg){

    LOG_DEBUG<<"cb1 has been callback";
    //sched.stop();
    return nullptr;
}




int main(){

    limlog::setLogFile("./test_log_file");
    limlog::setLogLevel(limlog::LogLevel::DEBUG);
    limlog::setRollSize(64); // 64MB
    LOG_DEBUG <<"start";
    sched.async_start();
    sched.addtask(cb1,nullptr,"test");
    sched.wait_all();
    return 0;

}
