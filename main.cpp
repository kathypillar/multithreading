#include <iostream>
#include "inc/sorting.inc.cpp"
#include "inc/threads.inc.cpp"

using namespace std;

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

	testbench("data.txt",bubSort,12,120000,0,256);
	testbench("data.txt",bubSort,12,1200000,1,256);
	testbench("data.txt",inSort,12,120000,0,256);
	testbench("data.txt",inSort,12,1200000,1,256);
	testbench("data.txt",mergeSort,12,1200000,0,256);
	testbench("data.txt",mergeSort,12,1200000,1,256);
	testbench("data.txt",cppSort,12,1200000,0,256);
	testbench("data.txt",cppSort,12,1200000,1,256);
}
