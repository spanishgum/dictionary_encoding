#include <iostream>
#include <string>
#include <cstdlib>

#include "trie.h"
#include "bplus.h"

class Dict 
	: public Trie, public Bplus<unsigned int, Trie::Node> 
{
	typedef Trie::Node TNode;
	typedef Bplus::Node BNode;
	
	private:
	
		Trie trie;
		Bplus<unsigned int, Trie::Node> bplus;
		
		unsigned int size;
		void insert(std::string, unsigned int);
	
	public:
	
		Dict();
		~Dict();
		
		unsigned int insert(std::string);
		void remove(std::string);
		
		void remove(unsigned int);
		int locate(std::string);
		std::string extract(unsigned int);
		
		void clear();
		void show();
		
		void serialize(std::string);
		void deserialize(std::string);
};
