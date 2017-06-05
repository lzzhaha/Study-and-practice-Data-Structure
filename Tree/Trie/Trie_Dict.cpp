#include "Tri_Dict.h"
#include <cstdlib>

//Default constructor
Dictionary::Dictionary(){
	root = create();
}

//Default destructor
Dictionary::~Dictionary{
	removeTrie(root);
}


/*
	Function:create
	Create a Trie node and initialize a dynamic 
	array of Trie node pointer, which stores the 
	descendant nodes.
	@parameter: 1. the word; 2. the definition of the word
	@return address of the newly created Trie node.
*/
Trie Dictionary::create(string word = "", string definition = ""){
	Trie new_node = new TrieNode;

	new_node->word = word;
	new_node->definition = definition;
	for(auto child:children){
		child = NULL;
	}
	
	return new_node;
}

/*
	Function:lookup
	lookup the word in the trie by comparing 
	each characters and move along the trie nodes
	accordingly. 
	@paramete: the word that is to be looked up, all
		the uppercase letters will be converted  
		to lower case.
	@return: a notification string if no such word is found,
		or the founded word appended with its definition.
*/
string Dictionary::lookup(string word){
	
	uint32_t length = word.length();
	
	Trie search = root;
	for(uint32_t i = 0; i<length; i++){
		word[i] = toLower(word[i]);
		uint16_t index = toInt(word[i]);
		
		if(search->children[index] == NULL){
			return string("There is no " + word + " in this dictionary\n");
		}else{
			search = search->children[index];
		}
	}

	if(search->definition == ""){ 
		return string("There is no " + word + " in this dictionary\n");
	}

	return string(word + search->definition);
}

/*
	Function:validateStr
	convert the input string to lower-case so as
	to insert it in the trie later on. Throw NonLetterErr
	if non-alphabetic error is encountered.
	@parameter: the word that is to be converted
*/
void Dictionary::validateStr(string& word){
	uint32_t length = word.length();

	for(uint32_t i =0; i<length; i++){
		word[i] = toLower(word[i]);
	}

}

/*
	Function: removeTrie
	Deallocate the memory of the Trie
	@parameter: the trieNode that is to
	be deleted
*/
void Dictionary::removeTrie(Trie& del){
	if(del == NULL){
		return;
	}

	for(auto child:del->children){
		removeTrie(child);
	}
	
	delete del;
	del = NULL;

}

/*
	Function: insertTrie
	Insert a word and its definition into the dictionary,
	new nodes are created along the insertion. Throw NonLetterErr
	if non-alphabetic error is encountered.
	@parameter: 1. the word to be inserted; 2. definition of the word
	@return: true if insertion is successful, false otherwise.
*/
bool Dictionary::insertTrie(string _word, string _definition){
	bool success = false;
	Trie insert_node = root;

	try{
		validateStr(_word);
	}catch(NonLetterErr e){
		std::cout<<e.what()<<std::endl;
		std::exit(1);
	}

	string check("");
	uint32_t index = 0;
	uint32_t length = _word.length();
	for(uint_32_t i = 0; i<length; i++){
		index = toInt(_word[i]);
		check += _word[i];
		if(insert_node->children[index] == NULL){
			insert_node->children[index] = create(check);
			if(i == length-1){
				//the word is complete
				insert_node->children[index]->definition = _definition;
				success = true;
				break;
			}
		}else{
			if(i == length-1){
				//the word is complete
				if(insert_node->children[index]->definition == NULL){
					//the node has empty definition, then add the definition
					insert_node->children[index]->definition = _definition;
				}else{
					//the node already has an definition
					std::cout<<"the dictionary already has the word: "<<_word<<std::endl;
					break;
				}
			}
		}
		insert_node = insert_node->children[index];
		
	}
	
	return success;
}

/*
	Function: modify
	modify the definition of an existing
	word in the dictionary, and NonLetterErr
	will be thrown if non-alphabetic letter
	is encountered.
	@parameter: 1.the word to be modified;
		    2.the new definition of the word;
	@return: true if the modification is successful, false otherwise
*/

bool Dictionary::modify(string _word, string _definition){
	bool success = false;
	Trie search = root;

	try{
		validateStr(_word);
	}catch(NonLetterErr e){
		std::cout<<e.what()<<std::endl;
		std::exit(1);
	}
	
	uint32_t length = word.length();
	
	
	for(uint32_t i = 0; i<length; i++){
	
		uint16_t index = toInt(word[i]);
		
		if(search->children[index] == NULL){
			return success;
		}else{
			search = search->children[index];
		}
	}

	search->definition = _definition;

	success = true;
	return success;

}

/*
	Function: load
	populate the dictionary with a words from
	a txt file. The file is assumed to have the
	format as : <word> : <definition> \n
	@parameter: the file name of the txt file
	@return: true if the all the words from the
	the file is successfully loaded, false otherwise
*/
bool Dictionary::load(string fileName){
	ifstream inputFile(fileName.c_str());
	
	if(inputFile.fail()){return false;}

	string inWord, inDefin;
	
	bool success = true;
	while(!inputFile.eof()){
		getLine(inputFile,inWord, ':');
		getLine(inputFile, inDefin, '\n');
		if(insert(inWord, inDefin)){
			std::cout<<"the word: "<<inWord<<" is successfully inserted in the dicitonary"<<std::endl;
		}else{
			std::cout<<"the word: "<<inWord<<" cannot be inserted"<<std::endl;
			success = false;
		}
	}
	inputFile.close();
	return success;
}
