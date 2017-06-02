#ifndef STR_HASHTABLE_H
#define STR_HASHTABLE_H
/*
	A hash table where stores the string key and string element.
	Use separate chaining to resolve conflicts.
	Hash function is use 31 as the base number.
*/

#include <utility>
#include <string>
#include <vector>

using std::vector;
using std::string;

typedef std::pair<string,string>* entry;

class HashTable{
	private:
		vector<vector<entry>*> content;
		int size;
	public:
		HashTable(int size = 50);
		~HashTable();
		int hashCode(const string)const;
		const string* getElement(const string);
		void insert(const string key, const string value);
		void removeElement(const string);
		
};

#endif
