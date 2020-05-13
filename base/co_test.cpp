#include "coroutine.h"

coroutine_t main_co;
 
void* cb1(void *arg);
void* cb2(void *arg);
 std::string one_arg = "one";
std::string two_arg = "two";

coroutine_t one(cb1,&one, one_arg);
coroutine_t two(cb2,&two, two_arg);



int main(){
   
  
    one.co_resume();



}