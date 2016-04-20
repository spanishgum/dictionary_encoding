#include "dictionary.h"


Dict::Dict(std::string i, std::string o)
{
	this->ifile = i;
	this->ofile = o;
	this->size = 0;
	this->trie = new Trie(this->ifile, this->ofile);
	this->bplus = new Bplus();
}

Dict::~Dict() 
{
	this->clear();
	delete this->trie;
	delete this->bplus;
}

void Dict::insert(std::string s, unsigned int id) 
{
	TNode *tn = this->trie->insert(s, id);
	this->bplus->insert(id, *tn);
}

unsigned int Dict::insert(std::string s)
{
	this->insert(s, this->size);
	return this->size++;
}

void Dict::remove(std::string s) 
{
	TNode *tn = this->trie->find(s);
	if (this->bplus)
	{
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
	TNode *tn = this->trie->find(s);
	if (tn == NULL) 
		return -1;
	return tn->id;
}

std::string Dict::extract(unsigned int id) 
{
	TNode *tn = this->bplus->find(id);
	return this->trie->getString(tn);
}

void Dict::clear() 
{
	this->bplus->clear();
	this->trie->clear();	
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
