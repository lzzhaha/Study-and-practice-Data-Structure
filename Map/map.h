#ifndef MAP_H
#define MAP_H

/*
A binary search tree implementation for STL std::map.
The prev_iter marks the biggest node which is smaller than the current node,
while the next_iter marks the smallest node which is bigger than the 
current node.
*/


#include <utility>
#include <iostream>
#include <iterator>
#include <string>
#include <initializer_list>
#include <sstream>
#include <chrono>
#include <random>


template<typename Key, typename Value>
class Map{
	using const_Key = const Key;
	using const_Value = const Value;
	using entry = std::pair<const Key, Value>;
	using const_entry = const std::pair<const Key, Value>;

	public:
		Map();
		~Map();
		//lightweight proxy of const entry[]
		Map(std::initializer_list<entry>);
		
		
		class Node{
			public:
				Node* right;
				Node* left;
				Node* parent;
				Node* next_iter;
				Node* prev_iter;
				
				char type;
				entry* pair_object;
				
				Node();
				Node(const entry&);
				//use rvalue reference to avoid unnecessary copy				
				Node(entry&&);
				~Node();
				
		};
		class ConstIterator;
		class Iterator{
			public:
				Node *iter_node;
				//Conversion constructor
				Iterator(Node*);
				Iterator& operator++();
				Iterator operator++(int);
				Iterator& operator--();
				Iterator operator--(int);
				
				entry& operator*();
				bool operator==(const Iterator&);
				bool operator==(const ConstIterator&);
				bool operator!=(const Iterator&);
				bool operator!=(const ConstIterator&);
				
		};

		class ConstIterator{
			public:
				const Node* iter_node;
				//Conversion constructor			
				ConstIterator(const Node*);
				//Convert non-const Iterator to const one
				ConstIterator(const Iterator&);
				//Copy constructor
				ConstIterator(ConstIterator&);
				ConstIterator& operator++();
				ConstIterator operator++(int);
				ConstIterator& operator--();
				ConstIterator operator--(int);

					
				const_entry& operator*() const;
				bool operator==(const ConstIterator&)const;
				bool operator==(const Iterator&)const;
				bool operator!=(const ConstIterator&)const;
				bool operator!=(const Iterator&)const;
		};

		class ReverseIterator{
			public:
				Node* iter_node;
				//Conversion constructor
				ReverseIterator(Node* n);
				ReverseIterator& operator++();
				ReverseIterator operator++(int);
				ReverseIterator& operator--();
				ReverseIterator operator--(int);
				
				entry& operator*();
				bool operator==(const ReverseIterator&);
				bool operator!=(const ReverseIterator&);
		};

		Iterator insert(const_entry&);
		Iterator insert(const_entry&&);
		void erase(Iterator);
		void set_pointer_after_deletion(Iterator);
		void remove(const_Key);
		
		Node* find_insert_pos(Node*,const Key);
		void delete_all(Node*&);
		void delete_node(Node*&);
			
		//Copy constructor	
		Map(const Map&);
		void copy_map(const Node*);
		Map(Map&&);
		Map& operator=(const Map&);
		Map& operator=(Map&&);
		
		Iterator begin();
		ConstIterator begin()const;
		Iterator end();
		ConstIterator end()const;
		
		ReverseIterator rbegin();
		ReverseIterator rend();
		
		std::size_t size()const;
		bool empty()const;
		void clear();
		Iterator find(const_Key);
		ConstIterator find(const_Key)const;
		Value& at(const_Key);
		const_Value& at(const_Key)const;
		Value& operator[](const_Key&);
		bool operator==(const Map&)const;
		bool operator!=(const Map&)const;
		
	private:
		
		Node* root;
		Node* head;
		Node* tail;

		std::size_t length;
		Iterator insert_helper(Node*);			
	
		
};

//Default constructor
template<typename Key, typename Value>
Map<Key, Value>::Map():root(NULL),head(new Node()),tail(new Node()){}

//Initialization by initializer
template<typename Key, typename Value>
Map<Key,Value>::Map(std::initializer_list<entry> list){
	for(auto item:list){
		insert(item);
	}
}

//Copy constructor
template<typename Key, typename Value>
Map<Key,Value>::Map(const Map& other){
	copy_map(other.root);
}

//Helper functions which facilitates the copy constructor
template<typename Key, typename Value>
void Map<Key,Value>::copy_map(const Node* other){
	if(other == NULL){return;}
	insert(*other->pair_object);
	copy_map(other->left);
	copy_map(other->right);
}

//Move Constructor
template<typename Key, typename Value>
Map<Key,Value>::Map(Map&& other):root(other.root),head(other.head),tail(other.tail){
	other.root = other.head = other.tail = NULL;
}

template<typename Key, typename Value>
Map<Key,Value>::~Map(){
	delete_all(root);
	delete_node(head);
	delete_node(tail);
}

template<typename Key, typename Value>
void Map<Key,Value>::delete_node(Node*& n){
	if(n == NULL || n->pair_object == NULL){return;}

	if(n->type == 'h'){
		head->next_iter = tail;
		tail->prev_iter = head;
	}
	delete n->pair_object;
	delete n;
	n = NULL;
}

template<typename Key, typename Value>
void Map<Key, Value>::delete_all(Node*& n){
	if(n == NULL){return;}

	delete_all(n->left);
	delete_all(n->right);
	delete_node(n);
}

//Default constructor
template<typename Key, typename Value>
Map<Key,Value>::Node::Node():pair_object(new entry()),next_iter(NULL),prev_iter(NULL),type('\0'),parent(NULL),left(NULL),right(NULL){}

//Copy constructor
template<typename Key, typename Value>
Map<Key,Value>::Node::Node(const_entry& obj):pair_object(new entry(obj)),next_iter(NULL),prev_iter(NULL),parent(NULL),left(NULL),right(NULL){}

//Destructor
template<typename Key, typename Value>
Map<Key,Value>::Node::~Node(){
	delete pair_object;
}

//Assignment operator overloading
template<typename Key, typename Value>
Map<Key,Value>& Map<Key,Value>::operator=(const Map<Key,Value>& other){
	if(this != &other){
		this->delete_all(root);
		copy_map(other.root);
	}
	return *this;
}

//Move assignment operator overloading
template<typename Key, typename Value>
Map<Key,Value>& Map<Key,Value>::operator=(Map<Key,Value>&& other){
	if(this != &other){
		delete_all(root);
		delete_all(tail);
		delete_all(head);
		root = other.root;
		head = other.head;
		tail = other.tail;
		other.root = NULL;
		other.head = NULL;
		other.tail = NULL;
	}
	return *this;
}


//Iterator Constructor
template<typename Key, typename Value>
Map<Key,Value>::Iterator::Iterator(typename Map<Key,Value>::Node* node):iter_node(node){}

//Const iterator constructor
template<typename Key, typename Value>
Map<Key,Value>::ConstIterator::ConstIterator(const Node* node):iter_node(node){}

//Convert non-const iterator to const one
template<typename Key, typename Value>
Map<Key,Value>::ConstIterator::ConstIterator(ConstIterator& other):iter_node(other.iter_node){}

//Copy constructor
template<typename Key, typename Value>
Map<Key,Value>::ConstIterator::ConstIterator(const Iterator& other):iter_node(other.iter_node){}

//Reverse iterator constructor
template<typename Key, typename Value>
Map<Key,Value>::ReverseIterator::ReverseIterator(Node* node):iter_node(node){}


template<typename Key, typename Value>
typename Map<Key,Value>::Iterator& Map<Key,Value>::Iterator::operator++(){	
	iter_node = iter_node->next_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator& Map<Key,Value>::ConstIterator::operator++(){
	iter_node = iter_node->next_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator& Map<Key,Value>::ReverseIterator::operator++(){
	iter_node = iter_node->prev_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::Iterator Map<Key,Value>::Iterator::operator++(int a){	
	Iterator temp(*this);
	++(*this);	
	return temp;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator Map<Key,Value>::ConstIterator::operator++(int a){	
	ConstIterator temp(*this);
	++(*this);	
	return temp;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator Map<Key,Value>::ReverseIterator::operator++(int a){	
	ReverseIterator temp(*this);
	++(*this);	
	return temp;
}

template<typename Key, typename Value>
typename Map<Key,Value>::Iterator& Map<Key,Value>::Iterator::operator--(){
	iter_node = iter_node->prev_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator& Map<Key,Value>::ConstIterator::operator--(){
	iter_node = iter_node->prev_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator& Map<Key,Value>::ReverseIterator::operator--(){
	iter_node = iter_node->next_iter;
	return *this;
}

template<typename Key, typename Value>
typename Map<Key,Value>::Iterator Map<Key,Value>::Iterator::operator--(int a){	
	Iterator temp(*this);
	--(*this);	
	return temp;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator Map<Key,Value>::ConstIterator::operator--(int a){	
	ConstIterator temp(*this);
	--(*this);	
	return temp;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator Map<Key,Value>::ReverseIterator::operator--(int a){	
	ReverseIterator temp(*this);
	--(*this);	
	return temp;
}

template<typename Key, typename Value>
bool Map<Key,Value>::Iterator::operator==(const Iterator& other){
	return (this->iter_node == other.iter_node);
}

template<typename Key, typename Value>
bool Map<Key,Value>::Iterator::operator!=(const Iterator& other){
	return !((*this) == other);
}

template<typename Key, typename Value>
bool Map<Key,Value>::ConstIterator::operator==(const ConstIterator& other)const{
	return (this->iter_node == other.iter_node);
}

template<typename Key, typename Value>
bool Map<Key,Value>::ConstIterator::operator!=(const ConstIterator& other)const{
	return !((*this) == other);
}

template<typename Key, typename Value>
bool Map<Key,Value>::ReverseIterator::operator==(const ReverseIterator& other){
	return (this->iter_node == other.iter_node);
}


template<typename Key, typename Value>
bool Map<Key,Value>::ReverseIterator::operator!=(const ReverseIterator& other){
	return !((*this) == other);
}



template<typename Key, typename Value>
typename Map<Key,Value>::Iterator Map<Key,Value>::begin(){
	Iterator re(head->next_iter);
	return re;
}

template<typename Key, typename Value>
typename Map<Key,Value>::Iterator Map<Key, Value>::end(){
	Iterator re(tail);
	return re;
}


template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator Map<Key,Value>::begin()const{
	ConstIterator re(head->next_iter);
	return re;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ConstIterator Map<Key, Value>::end()const{
	ConstIterator re(tail);
	return re;
}


template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator Map<Key,Value>::rbegin(){
	ReverseIterator re(tail->prev_iter);
	return re;
}

template<typename Key, typename Value>
typename Map<Key,Value>::ReverseIterator Map<Key, Value>::rend(){
	ReverseIterator re(head);
	return re;
}

template<typename Key, typename Value>
typename Map<Key,Value>::entry& Map<Key,Value>::Iterator::operator*(){
	return *(iter_node->pair_obj);
}


template<typename Key, typename Value>
typename Map<Key,Value>::const_entry& Map<Key,Value>::ConstIterator::operator*()const{
	return *(iter_node->pair_obj);
}


template<typename Key, typename Value>
typename Map<Key,Value>::entry& Map<Key,Value>::ReverseIterator::operator*(){
	return *(iter_node->pair_obj);
}

template<typename Key, typename Value>
std::size_t Map<Key,Value>::size()const{
	return length;

}

template<typename Key, typename Value>
bool Map<Key, Value>::empty()const{
	return root == NULL;
}

template<typename Key, typename Value>
bool Map<Key, Value>::operator==(const Map<Key,Value>& other)const{
	if(length != other.length){
		return  false;
	}
	Iterator i = begin();
	Iterator j = other.begin();

	for(;i != end();i++,j++){
		if(*i != *j){
			return false;
		}
	}
	return true;
}

template<typename Key, typename Value>
bool Map<Key, Value>::operator!=(const Map<Key,Value>& other)const{
	return !(*this==other);
}


template<typename Key, typename Value>
typename Map<Key, Value>::Iterator Map<Key,Value>::find(const Key key){
	Node* search = root;
	while(search != NULL){
		if(search->pair_object->first == key){
			return Iterator(search);
		}else if(search->pair_object->first< key){
			search = search->right;
		}else{
			search = search->left;
		}
	}
	return end();
}



template<typename Key, typename Value>
typename Map<Key, Value>::ConstIterator Map<Key,Value>::find(const Key key)const{
	Node* search = root;
	while(search != NULL){
		if(search->pair_object->first == key){
			return Iterator(search);
		}else if(search->pair_object->first< key){
			search = search->right;
		}else{
			search = search->left;
		}
	}
	return end();
}


template<typename Key, typename Value>
Value& Map<Key,Value>::at(const_Key key){
	Iterator result = find(key);
	if(result == end()){
		std::cout<<"No such key exists in this map"<<std::endl;
	}else{
		return (*result).second;
	}
}



template<typename Key, typename Value>
typename Map<Key,Value>::const_Value& Map<Key,Value>::at(const_Key key)const{
	ConstIterator result = find(key);
	if(result == end()){
		std::cout<<"No such key exists in this map"<<std::endl;
	}else{
		return (*result).second;
	}
}

template<typename Key, typename Value>
Value& Map<Key,Value>::operator[](const_Key& key){
	ConstIterator result = find(key);
	if(result == end()){
		Value v;
		result = insert(std::make_pair(key,v));
	}
	return (*result).second;
}

template<typename Key, typename Value>
typename Map<Key,Value>::Node* Map<Key,Value>::find_insert_pos(Node* node, const Key key){
	if(node == NULL) return NULL;	
	if(node->pair_objecct->first == key){
		return node;
	}else if(node->pair_objecct->first < key){
		if(node->right == NULL){
			return node;
		}else{
			return find_insert_pos(node->right, key);
		}
	}else{
		if(node->left == NULL){
			return node;
		}else{
			return find_insert_pos(node->left, key);
		}
	}
}


template<typename Key, typename Value>
typename Map<Key,Value>::Iterator Map<Key,Value>::insert(const_entry& obj){
	Node* new_node = new Node(obj);

	Node* insertNode = find_insert_pos(root, obj->seond);
	if(insertNode == NULL){
		//create a new root for this empty tree map
		root = new_node;
		new_node->type = 'h';
		new_node->next_iter = tail;
		new_node->prev_iter = head;
		head->next_iter = new_node;
		tail->prev_iter = new_node;
		length++;
		return Iterator(root);
	}else{
		if(insertNode->first == obj.first){
			//the key already exists, then simply overwrites the value
			insertNode->second = obj.second;
			delete new_node;
			return Iterator(insertNode);
		}else if(insertNode->first < obj.first){
			//insert in the right child 
			insertNode->right = new_node;
			new_node->parent = insertNode;
			new_node->type = 'r';

			new_node->prev_iter = insertNode;
			new_node->next_iter = insertNode->next_iter;
			new_node->next_iter->prev_iter = new_node;
			insertNode->next_iter = new_node;
			length++;
			return Iterator(new_node);
		}else{
			//insert in the left child
			insertNode->left = new_node;
			new_node->parent = insertNode;
			new_node->type = 'l';
			
			new_node->prev_iter = insertNode->prev_iter;
			new_node->prev_iter->next_iter = new_node;
			new_node->next_iter = insertNode;
			insertNode->prev_iter = new_node;
			length++;
			return Iterator(new_node);
		}
	}
}

template<typename Key, typename Value>
void Map<Key,Value>::remove(const_Key key){
	Iterator rem = find(key);
	if(rem == end()){
	 std::cout<<"No such key exists in this map"<<std::endl;
	}else{
		erase(rem);
	}
}


template<typename Key, typename Value>
void Map<Key,Value>::erase(Iterator it){
	Iterator i = find((*(it.iter_node->pair_obejct)).key);
	if(i == end()){
		std::cout<<"No such key exists in this map"<<std::endl;
		return;
	}
	Node* target = i.iter_node;
	Node* parent = target->parent;
	length--;

	if(target->left == NULL && target->right == NULL){
		//Zero child case
		if(target->type == 'l'){
			parent->left = NULL;
		}else if(target->type == 'r'){
			parent->right = NULL;
		}else{
			//the target node is the root
			root = NULL;			
		}
	}else if(target->left == NULL || target->right == NULL){
		//One child case
		Node* child = (target->left != NULL)?target->left:target->right;
		child->parent = parent;
		if(target->type == 'l'){
			parent->left = child;
		}else if(target->type == 'r'){
			parent->right = child;
		}else{
			//the target node is the root
			root = NULL;			
		}
	}else{
		//Two child case
		entry* temp = target->pair_object;
		target->pair_object = target->next_iter->pair_object;
		target->next_iter->pair_object = temp;
		erase(target->next_iter); 
	}
	
	//adjust the iterator and delete the node
	target->prev_iter->next_iter = target->next_iter;
	target->next_iter->prev_iter = target->prev_iter;

	delete_node(target);
}



#endif


