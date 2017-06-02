
#include <iostream>
#include <vector>
#include <cstdlib>
#include "FiNode.h"
using std::cerr;
using std::endl;
using std::vector;
template<typename T>
class FiHeap{
	private:
		FiNode<T>* min;

	public:
		FiHeap();
		~FiHeap();
		//Perform deep copy
		const FiHeap<T>& operator=(const FiHeap<T>&);
		
		void insert(FiNode<T>*);
		void insert(const T);
         	FiNode<T>* find(const T);
		const FiNode<T>* getMin()const{return min;}
		void deleteMin();
		void deleteNode(const T);
		void decreaseKey(const T target,const T delta);
		void setKey(const T target, const T newVal);
		void merge(const FiHeap*);
	
 	
};

//Default constructor
template<typename T>
FiHeap<T>::FiHeap(){
	min = NULL;
}

//Destructor
template<typename T>
FiHeap<T>::~FiHeap(){
	delete min;
}

/*
	Overloaded assignment operator
	perform deep copy.
	@return this pointer
*/

template<typename T>
const FiHeap<T>& FiHeap<T>::operator=(const FiHeap& other){
	if(this != &other){
		delete this;
		min = new FiNode<T>(other->getMin());
		FiNode<T>* sibling = other->getMin()->right;
		while(sibling != other->getMin()){
			insert(sibling);
		}
	}
	return *this;
}

/*
	Function:insert
	Inserts a node into the heap by
	using merge operation.
*/

template<typename T>
void FiHeap<T>::insert(FiNode<T>* in){
	if(min == NULL){
		min = new FiNode<T>(in);
	}else{
		if(find(in->getValue()) != NULL){
			cerr<<"No duplicate node is allowed"<<endl;
			std::exit(1);
		}
		FiHeap* temp = new FiHeap();
		temp->insert(in);
		merge(temp);
	}
}

//Insert a node by passing a particular value
template<typename T>
void FiHeap<T>::insert(const T in){
	FiNode<T> in_node(in);
	insert(&in);
}


/*
	Function:find
	recursively search the heap to find 
	the mathced node.
	@return the target node
*/

template<typename T>
FiNode<T>* FiHeap<T>::find(const T target){
	FiNode<T>* temp = min;
	FiNode<T>* search;
	do{
		search = temp->findInChildren(target);
		if(search != NULL){
			return search;
		}
		temp = temp->right;
	}while(temp != min);
	return NULL;
}


/*
	Function:deleteMin
	1. remove the min node from the root cycle
	2. merge the children of the previous min node with the root cycle
	3.adjust the new root cycle;
*/

template<typename T>
void FiHeap<T>::deleteMin(){
	if(min == NULL){
		cerr<<"Empty heap cannot be deleted"<<endl;
		std::exit(1);
	}

	// Step 1.
	FiNode<T>* tempMin, temp, temp2;
	int roots =0;
	if(min->right == min && min->children == NULL){
		delete this;
		return;
	}else if( min->right == min){
		tempMin = NULL;
	}else{
		tempMin = tempMin->right;
	}
	min->unlink();
	

	//Step2.
	temp = min->child;
	do{
		if(tempMin == NULL){
			tempMin = new FiNode<T>(temp);
		}else{
			tempMin->appendNode(temp);
		}
		temp->parent = NULL;
		temp = temp->right;
	}while(temp == min->child);
	//Step3.

	temp = tempMin;
	do{
		roots++;
		temp = temp->right;
	}while(temp != tempMin);

	min = tempMin;
	
	//create the array to keep track of the root node degrees
	vector<int> rootDegrees (roots+1);
	for(auto& root : rootDegrees){
			root=0;
	}
	
	for(int i=0; i<roots+1; i++){
		tempMin = min;
		temp = tempMin;
		do{
			if(temp->getValue() < min->getValue()){
				min = temp;
			}
						
			if(temp->getDegree() ==i){
				if(rootDegrees[i] == 0){
					rootDegrees[i] = temp->getValue();
					temp = temp->right;
				}else{
					temp2 = find(rootDegrees[i]);
					if(temp2->getValue() <= temp->getValue()){
						temp2->addChild(temp);
						temp2 = temp->right;
						temp->unlink();
						temp = temp2;
					}else{
						rootDegrees[i] = temp->getValue();
						temp->addChild(temp2);
						if(temp == tempMin){
							tempMin = temp->left;
						}
						temp2->unlink();
						temp = temp->right;
					}
				}
			}else{
				temp = temp->right;			
			}			
		}while(temp != tempMin);
	}
}

/*
	Function: deleteNode
	Delete the node with specified value and then merge the children
	of it with the root cycle.
*/
template<typename T>
void FiHeap<T>::deleteNode(const T target){
	FiNode<T>* targetNode = find(target);
	if(targetNode == min){
		deleteMin();
	}else{
		FiNode<T>* delChild = targetNode->child;
		targetNode->unlink();
		if(delChild != NULL){
			do{
				min->appendNode(delChild);
				delChild = delChild-> right;
			}while(delChild != targetNode->child);
		}	
	}	
}

/*
	Function:decreaseKey
	decrese the target value by delta amount, where delta might 
	be negative.
	Algorithm:
	1.check whether the target node or the node with new traget
	value exist;
	2.check whether the new value violate the heap property;
	3. if violation does occur, unlink the node node, and move the 
	new node to root cycle.
*/

template<typename T>
void FiHeap<T>::decreaseKey(T target, T delta){
	FiNode<T> dNode = find(target);	

	if(dNode == NULL){
		cerr<<"No specified node foung"<<endl;
		std::exit(1);
	}

	T newVal = target - delta;

	if(find(newVal)!= NULL){
		cerr<<"A node with specified new value already exists"<<endl;
		std::exit(1);
	}
	dNode->setValue(newVal);
	
	//The target node is not a root, then check the heap property
	if(dNode->parent != NULL){
		if(dNode->getValue() < dNode->parent->getValue()){
			FiNode<T>* temp = dNode;			
			dNode = min->append(dNode);
			delete temp->unlink();
		}
	}
	if(dNode->getValue() < min->getValue()){
		min = dNode;
	}
}



/*
	Function:setKey
	Set the node with target value with a new key,
	use decrease key operation to maintain Heap property.
*/

template<typename T>
void FiHeap<T>::setKey(const T target, const T newVal){
	FiNode<T>* tNode = find(target);
	if(tNode == NULL){
		cerr<<"No such node found"<<endl;
		std::exit(1);
	}

	T delta = target - newVal;
	decreaseKey(target, delta);
}

/*
	Function: merge
	merge two heaps by connecting the root cycles together,
	then adjust the min key.
*/

template<typename T>
void FiHeap<T>::merge(const FiHeap* other){
	if(other == NULL || other->getMin() == NULL){
		cerr<<"Merging with an empty heap is not allowed"<<endl;
		std::exit(1);
	}
	
	FiNode<T>* oMin = other->getMin();
	do{
		min->append(oMin);
		oMin = oMin->right;
	}while(oMin != other->getMin());

	if(min->getValue() > other->getMin()->getValue()){
		min = find(other->getMin()->getValue());
	}
}
