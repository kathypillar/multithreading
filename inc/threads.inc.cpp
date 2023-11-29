#include <iostream>
#include <thread>
#include <chrono> //remember kids never ever write your own time libraries
#include <climits>
#include <algorithm>
#include <fstream>

const int NUM_CORES = 6;//thread::hardware_concurrency()/2; //thread::hardware_concurrency() measures the total number of threads the system supports divide by 2 for the number of logical cores,which is the faster implemenation. Becuase hyperthreading doesn't actually make anything faster it just allows every core to handle two threads at around half speed.
//don't have six cores or have more too bad sucker i gotta fix some math

std::thread* threads = new std::thread[NUM_CORES]; //NUM_CORES is how many cores your system supports

void genThreads(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen) {
	for(int i=0;i<NUM_CORES;i++)
		threads[i] = std::thread(func,arr+(i*(arrLen/NUM_CORES)),arrLen/NUM_CORES+(arrLen%NUM_CORES));
	return;
}

void closeThreads() {
	for(int i=0;i<NUM_CORES;i++) {
		if(threads[i].joinable())
			threads[i].join();
	}
}

void mergeWrapper(int* start1,int* end1,int* end2) { //since inplace_merge is a cpp library you can't use a function pointer with it so I just made a wrapper
	std::inplace_merge(start1,end1,end2);
}

auto multiThread(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen,bool threaded) { //none of the sorting programs are in place just because that makes test benching more space efficient
	auto start = std::chrono::steady_clock::now();
	if(threaded) {
		genThreads(func,arr,arrLen); //use a divide and conquer scheme with threads
		closeThreads();
		int t = NUM_CORES/2;
		for(int j=t;j>0;j--) {
			int* start1;
			int* end2;
			int* end1;
			for(int i=0;i<j;i++) {
				start1 = arr+((i)*(2*(t-j+1))*(arrLen/NUM_CORES));
				end2 = start1+((2*(t-j+1))*(arrLen/NUM_CORES))+((i+1>=j)*(arrLen%NUM_CORES)); //mod to account for uneven arrays
				end1 = end2-(((t-j+1-(j==1)))*(arrLen/NUM_CORES))-((i+1>=j)*(arrLen%NUM_CORES));
				if((arr+arrLen)<end1)
				threads[i] = std::thread(mergeWrapper,start1,end1,end2);

			}
			closeThreads();
		}
	}
	else
	func(arr,arrLen);
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
}
