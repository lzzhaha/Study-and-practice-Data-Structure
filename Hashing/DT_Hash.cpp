#include "DT_Hash.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
using std::string;
using std::vector;
using std::cerr;
//Default Constructor

DT_HashTable::DT_HashTable():count(0),tableSize(11){
	array = new pair<Date,Name>*[tableSize];
	for(int i=0; i<tableSize; i++){
		array[i] = NULL;
	}

	string pline;
	ifstream file("primeNumbers.txt");
	if(file.is_open()){
		for(int i=0; getline(file, pline); i++){
			primes[i] = atoi(line.c_str());
		}
	}
}

//Destructor

DT_HashTable::~DT_HashTable(){
	for(int i = 0; i<tableSize; i++){
		delete array[i];
	}
	delete [] array;
}

//Get all the names corresponding to the parameter date

Name DT_HashTable::getNames(const Date& d)const{
	int index = find(d);

	if(index >= 0){
		return array[index]->second;
	}else{
		cerr<<"No such Date";
		std::exit(1);
	}
}

//Add a date-name pair to the hash table

void DT_HashTable::add(Date d, string name){
	if(count>0){
		if((size+1)/(float) tableSize >0.45f){
			resize();
		}
	}

	int code = hashCode(d, tableSize);

	Name v;
	v.push_back(name);
	pair<Date, Name> p(d,v);

	insert(p, array, code, 1, tableSize);
	count++;
}


//Utility function for insertion

void  DT_HashTable::insert(const pair<Date, Name>& p, 
std::pair<Date, Name>**& arr, int h, int i, int max){
	if(arr[h] == NULL){
		arr[h] = new pair<Date, Name>(p);
	}else{
		if(arr[h]->first == p.first){
			for(int j=0; j<p.second.size();j++){
				arr[h]->second.push_back(p.second[j]);
			}
		}else{
			//Use open addressing to resolve conflicts
			insert(p,arr,(h+2*i)% max, i+1, max);
		}
	}
}

//Find the index of the specified date
int DT_HashTable::find(const Date& d)const{
	int h = hashCode(d, tableSize);
	return find(b,h,1);
}

//search the probing sequence
int DT_HashTable::find(const Date&d, int h, int i)const{
	if(array[h] == NULL){
		std::cout<<"No such date in the hash table."<<std::end;		
		return -1;
	}else{
		if(array[h]->first == d){
			return h
		}else{
			find(d,(h+2*i)%tableSize, i+1);
		}
	}
}

//Generate hash code for a specified date

int DT_HashTable::hasCode(const Date& d, int max)const{
	int code = 13;
	code += pow(d.getYear()+1, 3);
	code += pow(d.getMonth()+1,2);
	code += pow(d.getDay()+1,1);

	code%=max;
	return code;
}

//resize the hash table to the next prime numbers

void DT_HashTable::resize(){
	int i=0;
	
	while((count+1)/(double)primes[i] > 0.45){
		i++;
	}

	int newSize = primes[i];
	resize(newSize);
}

//resize the hash array

void DT_HashTable::resize(int newSize){
	std::pair<Date,Name>** temp = new std::pair<Date, Name>*[newSize];

	for(int i=0; i<newSize; i++){
		temp[i] = NULL;
	}
	
	for(int  i=0; i<tableSize; i++){
		if(array[i]!=NULL){
			int index = hashCode(array[i]->first, newSize);
			insert(*array[i], temp, index,1, newSize);		
		}
	}


	for(int i=0; i<tableSize; i++){
		delete array[i];
	}
	delete[] array;
	array = temp;
	tableSize = newSize;
}



