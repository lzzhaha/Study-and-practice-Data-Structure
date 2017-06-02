#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include<iostream>

using std::cout;
using std::cin;

//get parent index 

inline int parent(int index){
	return index==0?0:(index-1)/2;
}

//Perform swap between elments
template<typename T>
void swap(T& a, T& b){
	T temp = a;
	a = b;
	b = temp;
}
//Heapify the current node as well as the decendent nodes by recursively making parents bigger than children
template<typename T>
void heapify(T arr[], int size, int index){
	int largest = index;
	int l_child = 2*index+1;
	int r_child = 2*index+2;
	
	if(l_child<size && arr[l_child]>arr[largest]){
		largest = l_child;
	}
	if(r_child<size && arr[r_child]>arr[largest]){
		largest = r_child;
	}	
	
	if(largest != index){
		swap(arr[index],arr[largest]);
		//make sure the corresponding decendent nodes are all adjusted properly
		heapify(arr,size,largest );	
	}
}

//Perform heapSort
template<typename T>
void heapSort(T arr[], int size){
	//Make the heap
	for(int i = parent(size-1);i >= 0;i-- ){
		heapify(arr, size, i);
	}

	//Sort the heap by reducing the size recursively
	for(int i = size-1; i > 0;i--){
		//cout<<"before swapping"<<std::endl;
		//cout<<"arr[0]: "<<arr[0]<<"arr[i] "<<arr[i]<<std::endl;
		swap(arr[0],arr[i]);
	//	cout<<"after swapping"<<std::endl;
          //      cout<<"arr[0]: "<<arr[0]<<"arr[i] "<<arr[i]<<std::endl;

		heapify(arr,i,0);
	}
}

#endif
