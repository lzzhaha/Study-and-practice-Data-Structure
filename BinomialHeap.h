#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <list>
#include <iostream>
using std::list;
using std::cout;

template<typename T>
struct Node{
	T key;
	int degree;
	Node<T>* child;//the left most child 
	Node<T>* sibling; //the  right sibling
	Node<T>* parent;

	Node(T k):key(k){
		degree = 0;

		child = sibling = parent = NULL;
	}

	
};

//Merge two binomial trees by taking two roots of two trees
template<typename T>
Node<T>* mergeTrees(Node<T>* t1, Node<T>* t2){
	//make the tree with smaller key and let t2 be the leftmost child of t1
	if(t1->key > t2->key){
		Node<T>* temp = t1;
		t1 = t2;
		t2 = temp;
	}

	t2->parent = t1;
	t2->sibling = t1->child;
	t1->child = t2;
	t1->degree++;

	return t1;
}

template<typename T>
using heap = list<Node<T>*>;

// Perform union on two Binary heap, which is represented as list of Node*
template<typename T>
heap<T> heapUnion(const heap<T>& h1, const heap<T>& h2){
	//create a new heap by putting each node in non-decreasing order	
	heap<T> newHeap;
	
	heap<T>::const_iterator it1 = h1.begin();
	heap<T>::const_iterator it2 = h2.begin();
	while(it1 != h1.end() && it2 != h2.end()){
		if((*it1)->degree <= (*it2)->degree){
			newHeap.push_back(*it1);
		}else{
			newHeap.push_back(*it2);
		}
	} 

	if(it1!=h1.end()){
		while(it1!=h1.end()){
			newHeap.push_back(*it1);	
		}
	}else{
		while(it2!=h2.end()){
			newHeap.push_back(*it2);
		}
	}

	return newHeap;
}

/*
	this function rearranges the newly created  heap
	so that the heap won't have any two tree roots 
	of the same degree
*/

template<typename T>
void heapify(heap<T>& h){
	if(heap<T>.size()<=1){
		return h;
	}

	heap<T>::iterator prev = cur = next = h.begin();	
	next = ++cur;
	next++;

	while(prev != h.end()){
		//if only one binomial tree remains
		if(cur == h.end()){
			break;
		}

		/*
		if cur and prev has different degree, no need to 
		merge, move to next trees
		*/

		if((*cur)->degree > (*prev)->degree){
			cur++;
			prev++;
			if(next != h.end()){
				next++;
			}
		}

		//degree of 3 consecutive trees are the same, move to the next
		else if(next != h.end() && (*next)-> degree = (*cur)->degree){
			cur++;
			next++;
			prev++;
		}
		//merge two trees with the same degree
		else{
			*prev = mergeTrees(*prev, *cur);
			h.erase(cur);
			cur=next;
			if(next != h.end()){
				next++;
			}		
		}
	}	
	
}

//Insert a binomial tree into binomial heap

template<typename T>
heap<T> insertTree(const heap<T>& h,const Node<T>* tree){
	heap<T> temp;
	temp.push_back(tree);
	temp = heapUnion(h, temp);
	heapify(temp);
	return temp;
}

/*
	remove the min key form a biomial tree,
	and then transform this tree into a
	binomail heap
*/

template<typename T>
heap<T> removeMinFromTree(const Node<T>* tree){
	heap<T> temp;
	if(tree == NULL){
		return temp; 
	}

	Node<T>* ptr1 = tree->child;
	Node<T>* ptr2 ;
	while(ptr1 != NULL){
		ptr2 = ptr1;
		ptr1 = ptr1->siblings;
		ptr2->siblings = NULL;
		temp.push_front(ptr2);
	}
	return temp;
}

//insert a key into the binomial heap

template<typename T>
heap<T> insert(heap<T> h, T key ){
	Node* temp = new Node(key);
	return insertTree(h,temp);
}

//return the pointer of min value in the  binomial heap
template<typename T>
const Node<T>* getMin(const heap<T>* h){
			heap<T>::const_iterator it = h.begin();
			const Node<T>* min = *(h.begin());
			for(;it != h.end(); it++){
				if((*it)->key < min->key){
					min = (*it);
				}	
			}	
			return min;
		}

//Extract and remove the min value from the binomial heap

template<typename T>
heap<T> extractMin(heap<T> h){
	heap<T> newHeap;
	const Node<T>* min = getMin(h);
	for(heap<T>::iterator it = h.begin();it!=h.end();it++{
		if(*it == min){
			continue;
		}else{
			newHeap.push_back(*it);
		}
	}
	heap<T> removedHeap = removeFromTree(min);
	newHeap = heapUnion(removedHeap, newHeap);
	heapify(newHeap);
	return newHeap;
}
#endif
