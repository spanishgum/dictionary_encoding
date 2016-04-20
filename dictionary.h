/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "trie.h"
*
*     This header file
*
*
*
*
*
*
*
*/

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
		
		Trie *trie;
		Bplus<unsigned int, Trie::Node> *bplus;
		
		unsigned int size;
		void insert(std::string, unsigned int);
		
		std::string ifile, ofile;
		bool fileExists(std::string);
		
		inline bool status_good()
		{
			return (this->trie && this->bplus);
		}
		
		
	public:
		
		Dict();
		Dict(std::string);
		Dict(std::string, std::string);
		~Dict();
		
		unsigned int insert(std::string);
		void remove(std::string);
		void remove(unsigned int);
		
		int locate(std::string);
		std::string extract(unsigned int);
		
		void clear();
		void show();
		
		void serialize(std::string);
		void serialize(std::ofstream&);
		void deserialize(std::string);
		void deserialize(std::ifstream&);
};
