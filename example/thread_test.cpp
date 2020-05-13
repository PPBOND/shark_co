#include "thread.h"
#include<iostream>
using namespace std;



void func() {
	cout << "in thread:" << thread_t::thread_tid()<<endl;
}

int main() {
	thread_t t1(func, "2222");
	thread_t t2(func,"3333");

	t1.start();
	t2.start();

	t1.join();	
	t2.join();
	cout << "in thread: " << thread_t::thread_tid();
	return 0;
}
