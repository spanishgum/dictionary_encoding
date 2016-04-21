/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "dictionary.cpp"
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

#include "dictionary.h"

#define DEFAULT_OUT "tests/dict.dat"


// Simple default constructor, set size to 1
//  because 0 is used as an error code
//  when an operation fails
Dict::Dict()
	: size(1), ifile(), ofile(DEFAULT_OUT)
{
	this->trie = new Trie();
	this->bplus = new Bplus<unsigned int, TNode>();
}

// An output file is specified for serialization
Dict::Dict(std::string o)
	: size(0), ifile(), ofile(o)
{
	this->trie = new Trie(this->ofile);
	this->bplus = new Bplus<unsigned int, TNode>();
}

// Both in and outs are specified
//  For in, we should check that the file exists
//  If the file does not exists, ignore it
Dict::Dict(std::string i, std::string o)
	: size(0), ifile(i), ofile(o)
{
	if (fileExists(this->ifile))
	{
		this->trie = new Trie(this->ifile, this->ofile);
	}
	else
	{
		this->trie = new Trie(this->ofile);
	}
	this->bplus = new Bplus<unsigned int, TNode>();
}

Dict::~Dict()
{
	this->clear();
	if (this->status_good())
	{
		delete this->bplus;
		delete this->trie;
	}
}


void Dict::insert(std::string s, unsigned int id)
{
	if (this->status_good())
	{
		TNode *tn = this->trie->insert(s, id);
		this->bplus->insert(id, *tn);
	}
}

unsigned int Dict::insert(std::string s)
{
	this->insert(s, this->size);
	return this->size++;
}

void Dict::remove(std::string s)
{
	if (this->status_good())
	{
		TNode *tn = this->trie->find(s);
		if (tn != nullptr)
		{
			this->bplus->remove(tn->id);
		}
	}	
}

void Dict::remove(unsigned int ui)
{
	if (this->bplus)
	{
		this->bplus->remove(ui);
	}
	// currently does not do anything regarding the trie
	//  need to find a way to get back the pointer to 
	//  the it finds (if it finds one)
}

// Searches the trie for s
// If found it returns the ID
// 0 is reserved as a ItemNotFound indicator
unsigned int Dict::locate(std::string s) 
{
	if (this->trie == nullptr) 
	{
		return 0;
	}
	TNode *tn = this->trie->find(s);
	if (tn == nullptr)
	{
		return 0;
	}
	return tn->id;
}

// Search the bplus trie for ID
// Dont worry about 0,
//  let the search fail at the benefit
//  of performance for other searches
std::string Dict::extract(unsigned int id) 
{
	TNode *tn = this->bplus->find(id);
	return this->trie->getString(tn);
}

void Dict::clear() 
{
	if (this->bplus != nullptr)
	{
		this->bplus->clear();
	}
	if (this->trie != nullptr)
	{
		this->trie->clear();
	}
}

// dictionary will basically show contents of both 
//  internal structures by calling their show elements
void Dict::show() 
{
	std::cout << "\nNum elements: " << this->size << "\n";
	if (this->bplus != nullptr)
	{
		std::cout << "\nBplus contents:\n";
		this->bplus->show();
	}
	if (this->trie != nullptr)
	{
		std::cout << "\nTrie contents:\n";
		this->trie->show();
	}
}

// (de)serialization methods simply call the trie which handles
//  everything under the hood
// Args are taken both as paths or streams
void Dict::serialize(std::string file)
{
	if (this->trie != nullptr)
	{
		this->trie->serialize(file);
	}
}

void Dict::deserialize(std::string file) 
{
	if (this->trie != nullptr)
	{
		this->trie->deserialize(file);
	}
}


