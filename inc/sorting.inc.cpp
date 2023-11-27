#pragma once
#include <iostream>
#include <thread>
#include <random>
#include <climits>
#include <algorithm>

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
	std::sort(arr,arr+arrLen); //a test of the default cpp sorting library
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

	std::inplace_merge(arr,arr+arrLen/2,arr+arrLen); //i realized over the course of writing my own merge function that cpp's is definetly better and i needed to use merge for another purpose so why not use the better one
}

void printArr(int* arr,unsigned long arrLen) {
	for(int i=0;i<arrLen;i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
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

int* junkData(unsigned long length,int seed) { //using an unsigned long becuase first of all arrays can't have a negative length and also I want to maximize the possible size
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> distrib(INT_MIN,INT_MAX);
	int* arr = new int[length];
	for(int i=0;i<length;i++)
		arr[i]=distrib(gen);
	return arr;
} //All this junk data preserved in an unfiltered state, growing at an alarming rate.
