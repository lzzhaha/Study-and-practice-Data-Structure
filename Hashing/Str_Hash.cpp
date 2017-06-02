#include <iostream>
#include "Str_HashTable.h"

using std::cout;
using std::endl;

//Construt a hashtable with specified size
HashTable::HashTable(int size):(this->size)(size){
	for(int i=0; i<size; i++){
		content.push_back(new vector<entry>);
	}
}

//Destructor
HashTable::~HashTable(){
	vector<vector<entry>*>::iterator it1;
	for(it1 = content.begin(); it1 != content.end(); it1++){
		vector<entry*>::iterator it2;
		for(it2 = (*it1).begin(); it2 != (*it1).end(); it2++){
			delete (*it2);
		}
		(*it1).clear();
		delete(*it1);
	}
	content.clear();
}

/*
	Function:hashCode
	compute the hash code for a given key by 
	applying the following formula:
	[31^(n-1)]*str[0] + [31^(n-2)]*str[1] +...+ str[n-1]
*/

int HashTable::hashCode(const string key){
	int code = 0;
	for(int i= 0; i < key.length(); i++ ){
		code += key.at[i] + 31*code;
	}
	return code;
}

/*
	Function:insert
	Insert the pair<string,string>* into the hash 
	table. If the string key is the same, then overwrites
	the entry after deleteing the previous entry.
*/

void HashTable::insert(const string key, const string value){
	int index = hashCode(key);

	if(content[index]->empty()){
		content[index]->push_back(new std::pair<string,string>);
		return;
	}

	vector<entry>::iterator it = content[index]->begin();
	for(;it != content[index]->end(); it++){
		if((*it)->first == key){
			(*it)->second = value;
			return;
		}
	}

	content[index]->push_back(new std::pair<string,string>);
}

//search for the item with specified key, return empty string if not found
//return a new string which has the same content as the target item
string* HashTable::getElement(const string key)const {
	int index = hashCode(key);
	
	if(content[index]->empty()){
		cout<<"No such entry"<<endl;
	}else{
		vector<entry>::iterator it = content[index]->begin();
		for(;it != content[index]->end(); it++){
			if((*it)->first == key){
				string* re = new string((*it)->second);
				return re;
			}
		}
	}
	return NULL;
}

//remove the element with specified key

void HashTable::removeElement(const string key){
	if(getElement(key) == NULL){
		cout<<"No such entry"<<endl;
		return;
	}
	
	int index = hashCode(key);
	vector<entry>::iterator it = content[index]->begin();
	for(;it != content[index]->end(); it++){
		if((*it)->first == key){
			content[index]->erase(it);
		}
	}
}

