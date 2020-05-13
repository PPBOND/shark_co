#include "coroutine.h"

coroutine_t main_co;
 
void* cb1(void *arg);
void* cb2(void *arg);
 std::string one_arg = "one";
std::string two_arg = "two";

coroutine_t one(cb1,&one, one_arg);
coroutine_t two(cb2,&two, two_arg);




void* cb1(void *arg){
    coroutine_t* test = (coroutine_t*) arg;
    printf("fuck you\n");
    two.co_resume();
    printf("hello %s\n",test->name().c_str());
    return nullptr;
}

void* cb2(void *arg){
    coroutine_t* test = (coroutine_t*) arg;
    printf("fuck you\n");
    printf("hello %s\n",test->name().c_str());
    return nullptr;
}
int main(){
   
  
    one.co_resume();



}