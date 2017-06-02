#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>
#include <cstdlib>
using std::cerr;
using std::exit;
using std::endl;


/*
Pri_Compare is a function class which compares the priority of two elements of
type T, return true if the first argument has higher priority than
second argument. This function class should have default constructor.
*/

template<typename T, typename pri_Compare> 
class Pque{
	private:
		T* elements;
		int count;
		int capacity;
		
		pri_Compare cmp;
		int parent(int index){return (index>0)?(index-1)/2:0;}
		void bubbleUp(int);
		void bubbleDown(int);
		void heapify();
		void swap(T&, T&);
		
	public:
		Pque();
		~Pque();
		Pque(const int capacity);
		Pque(const Pque&);
		void init(const int capacity);
		
		void insert(const T&);
		
		T extract_top_pri();
		T peek_top_pri() const;
		void remove_top_pri();

		int size() const{return count;}
		int getCapacity() const{return capacity;}
		bool empty(){return count == 0;}
		

};


template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::swap(T& a, T& b){
	T temp;
	temp = a;
	a = b;
	b = temp;
}

//Default constructor
template<typename T, typename pri_Compare>
Pque<T,pri_Compare>::Pque():elements(NULL),count(0),capacity(0){}

//Initialize a Priority Queue with specified capacity
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::init(const int capacity){
	//report error when encountering invalid input
	if(capacity < 0){
		cerr<<"capacity of a new Priority Queue must be non-negative"<<endl;
		exit(1);
	}

	this->capacity = capacity;
	count = 0;
	
	//clear the previous content 
	if(elements != NULL){delete [] elements;}
	elements = new T[capacity];
}

//A constructor that initializes a Priority Queue with specified capacity
template<typename T, typename pri_Compare>
Pque<T,pri_Compare>::Pque(const int capacity):elements(NULL){init(capacity);}

//Copy constructor that performs deep copy
template<typename T, typename pri_Compare>
Pque<T, pri_Compare>::Pque(const Pque<T, pri_Compare>& other):
capacity(other.capacity),count(other.count){
	elements = new T[count];	
	for(int i=0; i<count; i++){
		elements[i] = other.elements[i];
	}
}

//Destructor
template<typename T, typename pri_Compare>
Pque<T, pri_Compare>::~Pque(){
	delete [] elements;
}

//Insert a node into the priority queue, adjust the position by bubble up
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::insert(const T& in){
	if(count == capacity){
		cerr<<"No enough space for insertion"<<endl;
		exit(1);
	}

	elements[count++] = in;
	bubbleUp(count-1);
}

/*
	Function:bubbleUp
	compare the node with its parent and adjust its
	position by moving it up, so as to maintain the
	heap property
	@parameter: i is the index that requires operation
*/
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::bubbleUp(int i){
	int p = parent(i);
	while(i != 0){
		if(cmp(elements[i],elements[p])){
			swap(elements[i],elements[p]);
		}else{
			break;
		}
	}
}

/*
	Function:bubbleDown
	compare the node with its children  and adjust its
	position by moving it down. Replacement takes place along
	the way.
	@parameter: i is the index that requires operation
*/
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::bubbleDown(int i){
	int leftC = 2*i + 1;
	int root = i;
	if(leftC > count-1){
		return;
	}
	
	if(cmp(elements[leftC],elements[root])){
		root = leftC;
	}

	int rightC = 2*i + 2;
	if(rightC <= count-1 && cmp(elements[rightC],elements[root])){
		root = rightC;
	}
	
	if(root != i){
		swap(elements[root],elements[i]);
		bubbleDown(root);
	}
}

/*
	Function:heapify
	use bubbleDown from the bottom to the top
	so as to maintain the heap property
*/
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::heapify(){
	for(int i = count-1; i>=0;i--){
		bubbleDown(i);
	}
}

/*
	Function:extract_top_pri
	extract the elements with the highest priority,
	which is located at the index 0;
	report error if the PQ is empty;
*/

template<typename T, typename pri_Compare>

T Pque<T,pri_Compare>::extract_top_pri(){
	if(count == 0){
		cerr<<"The Priority Queue is empty"<<endl;
		exit(1);
	}
	
	T temp = elements[0];
	elements[0] = elements[--count];
	bubbleDown(0);
	return temp;
}

//get the elements with top priority without removing it
template<typename T, typename pri_Compare>
T Pque<T,pri_Compare>::peek_top_pri() const{
	if(count == 0){
		cerr<<"The Priority Queue is empty"<<endl;
		exit(1);
	}
	return elements[0];
}

//remove the elements with top priority
template<typename T, typename pri_Compare>
void Pque<T,pri_Compare>::remove_top_pri(){
	if(count == 0){
		cerr<<"The Priority Qendueue is empty"<<endl;
		exit(1);
	}
	
	elements[0] = elements[--count];
	bubbleDown(0);
}


#endif
