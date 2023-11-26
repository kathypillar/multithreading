#include <iostream>
#include <thread>
#include <chrono> //remember kids never ever write your own time libraries
#include <random>
#include <climits>
#include <algorithm>
#include <fstream>

using namespace std;
const int NUM_CORES = 6;//thread::hardware_concurrency()/2; //thread::hardware_concurrency() measures the total number of threads the system supports divide by 2 for the number of logical cores,which is the faster implemenation. Becuase hyperthreading doesn't actually make anything faster it just allows every core to handle two threads at around half speed.
//don't have six cores or have more too bad sucker i gotta fix some math

thread* threads = new thread[NUM_CORES]; //NUM_CORES is how many cores your system supports

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


void bubSort(int* arr,unsigned long arrLen) {
	int temp;
	for(int i=0;i<arrLen-1;i++) {
		for(int j=0;j<(arrLen-i-1);j++) {
			if(arr[j+1]<arr[j]) {
				temp=arr[j+1];
				arr[j+1]=arr[j];
				arr[j]=temp;
			}
		}
	}

}
void cppSort(int* arr,unsigned long arrLen) {
	sort(arr,arr+arrLen); //a test of the default cpp sorting library
}

void inSort(int* arr,unsigned long arrLen) {
	int k,j;
	for(int i=1;i<arrLen;i++) {
	k=arr[i];
	j=i-1;
		for(int h=0;j>=0&&arr[j]>k;j--)
			arr[j+1]=arr[j];
		arr[j+1]=k;
	}

}

void mergeSort(int* arr,unsigned long arrLen) {
	if(arrLen==1)
		return;
	mergeSort(arr,arrLen/2);
	mergeSort(arr+(arrLen/2),(arrLen/2)+(arrLen%2));

	inplace_merge(arr,arr+arrLen/2,arr+arrLen); //i realized over the course of writing my own merge function that cpp's is definetly better and i needed to use merge for another purpose so why not use the better one
}

void printArr(int* arr,unsigned long arrLen) {
	for(int i=0;i<arrLen;i++)
		cout << arr[i] << " ";
	cout << endl;
}

bool checkArr(int* arr,unsigned long arrLen,int* arr2, unsigned long arrLen2) {
	if(arrLen!=arrLen2)
		return false;
	for(int i=0;i<arrLen;i++) {
		if(arr[i]!=arr2[i])
			return false;
	}
	return true;
}

void mergeWrapper(int* start1,int* end1,int* end2) { //since inplace_merge is a cpp library you can't use a function pointer with it so I just made a wrapper
	inplace_merge(start1,end1,end2);
}

auto multiThread(void (*func)(int*,unsigned long),int* arr,unsigned long arrLen,bool threaded) { //none of the sorting programs are in place just because that makes test benching more space efficient
	auto start = chrono::steady_clock::now();
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
				if((arr+arrLen)<end1) //TODO this logic only works for 6 cores
				threads[i] = thread(mergeWrapper,start1,end1,end2);

			}
			closeThreads();
		}
	}
	else
	func(arr,arrLen);
	auto end = chrono::steady_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(end-start).count();
}

int* junkData(unsigned long length,int seed) { //using an unsigned long becuase first of all arrays can't have a negative length and also I want to maximize the possible size
	mt19937 gen(seed);
	uniform_int_distribution<> distrib(INT_MIN,INT_MAX);
	int* arr = new int[length];
	for(int i=0;i<length;i++)
		arr[i]=distrib(gen);
	return arr;
} //All this junk data preserved in an unfiltered state, growing at an alarming rate.

void testbench(string fileName,void (*func)(int*,unsigned long),unsigned long minSize,unsigned long maxSize,bool threaded,int seed) {
	ofstream file;
	file.open(fileName);


	if(func==bubSort)
		file << "BubSort:" << endl;
	if(func==inSort)
		file << "InSort:" << endl;
	if(func==mergeSort)
		file << "MergeSort:" << endl;
	if(func==cppSort)
		file << "CppSort:" << endl;

	if(threaded)
		file << "Threaded:" << endl;
	else
		file << "Unthreaded:" << endl;

	for(int i=minSize;i<=maxSize;i=i*10) {
		int* data = junkData(i,seed);
		file << i << ": "<<  multiThread(bubSort,data,i,threaded) << endl;
		delete[] data;
	}
}

int main() {

	testbench("data.txt",inSort,12000,12000,1,256);
	testbench("data.txt",bubSort,12,1200000,1,256);
	testbench("data.txt",inSort,12,120000,0,256);
	testbench("data.txt",inSort,12,1200000,1,256);
	testbench("data.txt",mergeSort,12,1200000,0,256);
	testbench("data.txt",mergeSort,12,1200000,1,256);
	testbench("data.txt",cppSort,12,1200000,0,256);
	testbench("data.txt",cppSort,12,1200000,1,256);
}
