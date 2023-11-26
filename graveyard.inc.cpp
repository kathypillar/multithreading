//this file is worthless, just a showcase of failed ideas

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <climits>
#include <algorithm>
#include <fstream>
#include <future>
#include <queue>
#include <mutex>
#include <vector>

 //Look on my works, ye Mighty, and despair! I now know what a mutex and thread pool are due to my mistakes at least
struct task { //each task is a struct with the sorting method and the array
	void (*func)(int*,int*,unsigned long);
	int* arr;
	int* arrCopy;
	unsigned long arrLen;
};
queue<task> toDo; //queue of tasks
mutex toDoLock; //makes sure a race condition doesn't happen
condition_variable lockStatus;

void purgatory() { //every thread is trapped in here
	task t;
	while(1){
		{
		unique_lock<mutex> lock(toDoLock);
		lockStatus.wait(lock, []{return !toDo.empty();});
		t=toDo.front();
		toDo.pop();
		}
		t.func(t.arr,t.arrCopy,t.arrLen);
	}
}

void threadPool() { //don't use thread pools
	thread* threads = new thread[NUMTHREADS]; //NUMTHREADS is how many threads your system supports
	for(int i=0;i<NUMTHREADS;i++)
		threads[i] = thread(purgatory); //init all threads to be in purgatory
	return;
}

void addTask(task t) {
	unique_lock<mutex> lock(toDoLock);
	toDo.push(t);
	lock.unlock();
	lockStatus.notify_one();
}

void null(int* a,int* b, unsigned long c){}

bool working() {
	bool b;
	unique_lock<mutex> lock(toDoLock);
	b=!toDo.empty();
	lock.unlock();
	return b;
}
*/

thread* threads = new thread[NUM_CORES]; //NUM_CORES is how many cores your system supports

void garbageSort(int* arr,unsigned long arrLen) {
	return;
}


void genThreads(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen) {
	for(int i=0;i<NUM_CORES;i++)
		threads[i] = thread(func,arr+(i*(arrLen/NUM_CORES)),arrLen/NUM_CORES+(arrLen%NUM_CORES));
	return;
}

void closeThreads() {
	for(int i=0;i<NUM_CORES;i++) {
		if(threads[i].joinable())
			threads[i].join();
	}
}
