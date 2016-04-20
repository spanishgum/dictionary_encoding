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
*     This source file
*
*
*
*
*
*
*
*/

#include "dictionary.h"


Dict::Dict()
{
	this->ifile = "";
	this->ofile = "";
	this->size = 0;
	this->trie = new Trie();
	this->bplus = new Bplus<unsigned int, Trie::Node>();
}

Dict::Dict(std::string o)
{
	this->ifile = "";
	this->ofile = o;
	this->size = 0;
	this->trie = new Trie(this->ofile);
	this->bplus = new Bplus<unsigned int, Trie::Node>();
}

Dict::Dict(std::string i, std::string o)
{
	this->ifile = i;
	this->ofile = o;
	this->size = 0;
	if (fileExists(this->ifile))
	{
		this->trie = new Trie(this->ifile, this->ofile);
	}
	else
	{
		this->trie = new Trie(this->ofile);
	}
	this->bplus = new Bplus<unsigned int, Trie::Node>();
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

// Simple utility to chech if a file exists
bool Dict::fileExists(std::string path)
{
	std::ifstream ifs;
	ifs.open(path, std::ios::in | std::ios::binary);
	if (!ifs)
	{
		return false;
	}
	ifs.close();
	return true;
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
		this->bplus->remove(tn->id);
	}	
}

void Dict::remove(unsigned int ui)
{
	if (this->bplus)
	{
		this->bplus->remove(ui);
	}
}

int Dict::locate(std::string s) 
{
	if (!this->trie) 
	{
		return -1;
	}
	TNode *tn = this->trie->find(s);
	if (tn == NULL)
	{
		return -1;
	}		
	return tn->id;
}

std::string Dict::extract(unsigned int id) 
{
	TNode *tn = this->bplus->find(id);
	return this->trie->getString(tn);
}

void Dict::clear() 
{
	if (this->bplus)
	{
		this->bplus->clear();
	}
	if (this->trie)
	{
		this->trie->clear();
	}
}

void Dict::show() 
{
	this->bplus->show();
}


void Dict::serialize(std::string file)
{
	this->trie->serialize(file);
}

void Dict::serialize(std::ofstream& ofs)
{
	this->trie->serialize(ofs);
}

void Dict::deserialize(std::string file) 
{
	this->trie->deserialize(file);
}

void Dict::deserialize(std::ifstream& ifs) 
{
	this->trie->deserialize(ifs);
}
