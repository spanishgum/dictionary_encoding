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
		
		inline bool status_good() const
		{
			return ((this->trie != nullptr)
				&& (this->bplus != nullptr));
		}
		
	public:
		
		Dict();
		Dict(std::string);
		Dict(std::string, std::string);
		~Dict();
		
		static bool fileExists(std::string path)
		{
			std::ifstream ifs;
			ifs.open(path);
			if (!ifs)
			{
				return false;
			}
			ifs.close();
			return true;
		}		
		
		// user inserts a string and gets the unique uint back
		unsigned int insert(std::string);
		
		// removing a mapped pair by either element
		void remove(std::string);
		void remove(unsigned int);
		
		// finding a mapped pair by either element 
		unsigned int locate(std::string);
		std::string extract(unsigned int);
		
		// reset the dictionary
		void clear();
		
		// attempt a makeshift console diagram of the tree
		void show();
		
		// (de)serialization methods both by streams or paths
		void serialize(std::string);
		void serialize(std::ofstream&);
		void deserialize(std::string);
		void deserialize(std::ifstream&);
};
