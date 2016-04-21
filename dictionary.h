/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "dictionary.h"
*
* This project explores the idea of creating a 2-way hash map to promote:
*   2-way search performance
*   data compression
*   low memory overhead
*
* Large datasets from the DBpedia are be used to challenge our software
*   so that results can be compared to that of HDT
*
* While this project certainly met some level of the goals above,
*   HDT is a much more mature software solution which employs 
*   a series of advances techniques which were beyond the scope of this work.
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
		void deserialize(std::string);
};
