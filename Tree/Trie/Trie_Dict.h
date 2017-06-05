/*
	Implement a simple dictionary using Tried data structure.
	Support functionalities including: look-up words, 
	populate dictionary from txt files, insert words and definition and
	modification operation.
*/
#ifndef TRIE_DICT_H_
#define TRIE_DICT_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

using std::string;

//Define the exception object for non-letter error
class NonLetterErr:std::exception{
	public :
		const char* what(){
			return "Invalid input words! All characters should be alphabetic letters!\n";
		}
};


class Dictionary{
	private:
		struct TrieNode{
			string word;
			string definition;
			TrieNode* children[26];	//26 letters
		};

		typedef TrieNode* Trie;

		Trie root;

		//utility functions
		Trie create(string word = "", string definition = "");
		//Convert lower_case letter to int, where 'a' correspond to 0
		int toInt(char c)const{ return c - 'a';}
		void write(Trie, std::fstream&);
		void validateStr(string&);
		void removeTrie(Trie&);

	public:
		Dictionary();
		~Dictionary();
		bool insertTrie(string word,string definition);
		string lookup(const string);
		bool writeTrie(string);
		void display()const;
		bool load(const string fileName);	
		bool modify(string,string);

		//Some helper function to handle characters
		static bool isSpace(const char c){return c == ' ';}
		
		static bool isUpper(const char c){return c >= 'A' && c <= 'Z';}
		
		static bool isLower(const char c){return c >= 'a' && c <= 'z';}		
		
		static bool isAlphabetic(const char c){return isUpper(c) || isLower(c);}

		static char toLower(const char c){
			if(isUpper){
				char re = static_cast<char>(c - 'A' + 'a');
				return re;	
			}else if(isLower){
				return c;
			}else{
				throw NonLetterErr();
			}
		}
};


#endif
