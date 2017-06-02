/*
	File: FiNode.cpp
	definition of FiNode class members
*/


template<typename T>
class FiNode{
	private:
		T value;
		bool marked;
		int degree;
		FiNode<T>* appendSameNode(FiNode<T>*);
	
	public:
		FiNode();
		FiNode(const FiNode<T>*);
		FiNode(T);
		~FiNode();

		FiNode* left, right, parent, child;
		
		const T& getValue() const;
		void setValue(T val){value = val;}
		/*for each non-root node, at most one child can be cut,
		and this node becomes marked after cutting one child. 
		Root node is always unmarked.
		*/
		bool isMarked() const {return marked;}
		void mark(){marked = true;}
		void unMark(){marked = false;}
		
		int getDegree() const{return degree;}
		FiNode* unlink();
		FiNode* addChild(const FiNode*);
		FiNode* addChild(const T);
		FiNode* append(const FiNode*);
		FiNode* preAppend(const FiNode*);
		FiNode* findInChildren(const T);

};



#include <iostream>
using std::cerr;
using std::endl;


//Default constructor
template<typename T>
FiNode<T>::FiNode(): parent(NULL), child(NULL),marked(false),degree(0){
	left = right = this;
}

//Conversion constructor
template<typename T>
FiNode<T>::FiNode(T val):value(val),parent(NULL),child(NULL),marked(false),degree(0){
	left = right = this;
}


//Copy constructor
template<typename T>
FiNode<T>::FiNode(const FiNode<T>* other):parent(NULL),degree(0),child(NULL){
	if(other == NULL){
		cerr<<"ERROR: Empty pointer cannot be used for construction!"<<endl;
		std::exit(1);
	}
	
	left = right = this;
	marked = other->isMarked();
	value = other->getValue();
	if(other->child!=NULL){	
	
		FiNode<T>* cur = other->child;	
		do{
			addChild(cur);
			cur = cur->right;
		}while(cur != other->child);
	}
}
//Destructor, recursively destruct the siblings and children of a node
template<typename T>
FiNode<T>::~FiNode(){
	if(child != NULL){
		delete child;
	}
	left->right = NULL;
	

	if(right != NULL){
		 delete right;
	}
	delete this;

}



//Return the value stored in the node
template<typename T>
const T& FiNode<T>::getValue()const{
	return value;
}

/*
	Function: unlink
	removes all connection of the current node except children.
	@return A pointer to the unlinked node
*/
template<typename T>
FiNode<T>* FiNode<T>::unlink(){
	FiNode* temp;
	FiNode* next;
	//The node has no siblings
	if(right == this){
		if(parent != NULL){
			//this node is not a root			
			parent->child = NULL;
			if(parent->isMarked()){
				// parent has been cut a child, needs to unlink the parent and put it to the root level 
				parent->unmark();
				temp = parent;
				next = temp->parent;
				while(next->parent != NULL){
					next = next->parent;
				}				
				next->append(temp);
				delete temp->unlink();

			}else if(parent->parent !=NULL){
				//parent node is not a root, then set it to marked				
				parent->mark();
			}
		}else{
			//this node is a root
			return this;
		}
	}else{
		//this node has sibling
		right->left = left;
		left->right = right;

		left = right = this;
		//this node is a root
		if(parent == NULL){
			return this;
		}else{
			//this node is not a root

			if(parent->child == this){
				parent->child = this->right;
			}

			if(parent->isMarked()){
				//parent has been cut a child, needs to unlink the parent and put it to the root level 
				parent->unmark();
				temp = parent;
				next = temp->parent;
				while(next->parent != NULL){
					next = next->parent;
				}
				next->append(temp);
				delete temp->unlink();
			}else if(parent->parent != NULL){
				//parent is not a root then set to marked				
				parent->mark();
			}
			parent =NULL;

		}
	}
	return this;
}

/*
	Function: append
	adds a sibling to the right of the node,
	which is a deep copy of the parameter.
	@return A pointer to the node
*/

template<typename T>
FiNode<T>* FiNode<T>::append(const FiNode<T>* ap){
	if(ap == NULL){
		cerr<<"ERROR: Appending a null node is not allowed"<<endl;		
		std::exit(1);
	}
	
	FiNode<T>* temp = new FiNode<T>(ap);
	temp->left = this;
	temp->right = right;	
	if(right == this){
		//the node has no siblings		
		left = temp;
	}else{
		//the node has siblings
		right->left = temp;
	}
	right = temp;
	return temp;
			
}

/*
	Function:appendSameNode
	adds a sibling to the right of the node, 
	which is a shallow copty of the parameter.
	@return A pointer to the node
*/

template<typename T>
FiNode<T>* FiNode<T>::appendSameNode(FiNode<T>* ap){
	if(ap == NULL){
		cerr<<"ERROR: Appending a null node is not allowed"<<endl;		
		std::exit(1);
	}
	ap->left = this;
	ap->right = right;
	if(right == this){
		//the node has no siblings		
		left = ap;
	}else{
		//the node has siblings
		right->left = ap;
	}
	right = ap;
	return ap;			
}

/*
	Function:preAppend
	adds a sibling to the left of the node,
	which is deep copy of the parameter.
	@return a pointer to the appended node.
*/
template<typename T>
FiNode<T>* FiNode<T>::preAppend(const FiNode<T>* ap){
	if(ap == NULL){
		cerr<<"ERROR: Appending a null node is not allowed"<<endl;		
		std::exit(1);
	}
	
	FiNode<T>* temp = new FiNode<T>(ap);
	temp->left = left;
	temp->right = this;
	if(right == this){
		//the node has no siblings		
		right = temp;
	}else{
		//the node has siblings
		left->right = temp;
	}
	left = temp;
	return temp;
}

/*
	Function: addChild
	Adds a child node to the node's children linked list
	@return a pointer to the new node
*/

template<typename T>
FiNode<T>* FiNode<T>::addChild(const FiNode<T>* newChild){
	if(newChild == NULL){
		cerr<<"ERROR: Adding a NULL node is not allowed"<<endl;
		std::exit(1);
	}
	
	degree++;
	if(child == NULL){
		child = new FiNode<T>(newChild);
		child->parent = this;
	}else{
		FiNode<T>* temp = child->append(newChild);
		if(newChild->child != NULL){
			temp->addChild(newChild->child);
		}
		temp->parent = this;
		return temp;
	}
}

//Add a new child by passing a value of node

template<typename T>
FiNode<T>* FiNode<T>::addChild(const T val){
	FiNode<T> child(val);
	return addChild(&child);
}

/*
	Function: findInChildren
	search for a particular value in the children nodes,
	use recursion to search each layer.
	@return the node with target value if found or NULL if not
	found.
*/

template<typename T>
FiNode<T>* FiNode<T>::findInChildren(const T target){
	if(value == target){
		return this;
	}
	if(child != NULL){
		FiNode<T>* temp = child;
		FiNode<T>* result;
		do{
			result = temp->findInChildren(target);
			if(result !=NULL){
				return result;
			}
			temp = temp->right;
		}while(temp!=child);
	}
	return NULL;
}
