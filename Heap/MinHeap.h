#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
#include <vector>
using std::vector;
using std::ostream;
using std::cout;
using std::endl;
template<typename T>
class MinHeap{
	
	friend ostream& operator<<(ostream&, const MinHeap&);
	private:
		vector<T> data;
		//utility functions to maintain the heap property
		void bubbleUp(int);
		void bubbleDown(int);
		void heapify();
		size_t parent(int index)const{
			return index==0?0:(index-1)/2;
		}
		void swap(T& a, T& b){
			T temp = a;
			a = b;
			b = temp;
		}
	public:
		MinHeap(){};
		MinHeap(const MinHeap<T>&);
		MinHeap(const T input[], int size);
		const T& getMin() const;
		void insert(const T&);
		//delete the element located at n-th slot
		void remove(int index);
		void deleteMin(){remove(0);}
		
		
};


template<typename T>
MinHeap<T>::MinHeap(const MinHeap<T>& other):data(other.data){}

template<typename T>
MinHeap<T>::MinHeap(const T input[], int size){
	
	for(int i=0;i<size;i++){
		data.push_back(input[i]);
	}
	heapify();
}

template<typename T>
void MinHeap<T>::bubbleUp(int index){
	int parentIndex = parent(index);
	if(data[parentIndex] > data[index]){
		swap(data[index],data[parentIndex]);
		bubbleUp(parentIndex);
	}
}

template<typename T>
void MinHeap<T>::bubbleDown(int index){
	
	int min = index;
	int lchild = 2*index+1;

	if(lchild >= data.size() ){
		//this node cannot have children
		return;
	}else if(data[lchild] < data[min]){
		min = lchild;
	}

	int rchild = 2*index+2;
	if(rchild<data.size() && data[rchild] < data[min]){
		min = rchild;
	}

	if(min != index){
		//recursively swap elements to ensure that parent is the smaller than its childrent
		swap(data[index], data[min]);
		bubbleDown(min);
	}
}

template<typename T>
void MinHeap<T>::remove(int index){
	int size = data.size();
	
	if(size==0){
		cout<<"the heap is empty!"<<endl;
	}
	
	data[index] = data[size-1];
	data.pop_back();
	bubbleDown(index);
}

template<typename T>
const T& MinHeap<T>::getMin()const{
	return data[0];
}

template<typename T>
void MinHeap<T>::insert(const T& element){
	data.push_back(element);
	bubbleUp(data.size()-1);
	
}


template<typename T>
void MinHeap<T>::heapify(){
	int size = data.size();
	//get the parent of last node;
	int beginIndex = parent(size-1);
	for(int i = beginIndex; i>=0; i--){
		bubbleDown(i);
	}
}


#endif
