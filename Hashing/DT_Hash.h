#ifndef DT_HASH_H
#define DT_HASH_H

#include "date.h"
#include <vector>
#include <utility>

typedef std::vector<std::string> Name;

class DT_HashTable{

	private:
		int primes[3244];
		int count;
		int tableSize;
		std::pair<Date, Name> **array;

	//utility functions
		void insert(const std::pair<Date, Name>&, 
		std::pair<Date, Name>** &, int, int, int);
		int find(const Date&, int, int)const;
		int find(const Date&)const;
	
	public:
		DT_HashTable();
		~DT_HashTable();
		Name getNames(const Date&)const;
		int getCount()const{return count;}
		void addEntry(Date, std::string);
		int hashCode(const Date&, int)const;
		void resize();

};

#endif
