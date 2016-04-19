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

void Dict::remove(std::string) 
{

}

void Dict::remove(unsigned int)
{

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
	std::cout << "Clearing bplus\n";
	this->bplus->clear();
	std::cout << "Clearing trie\n";
	this->trie->clear();
	std::cout << "Dict is cleared\n";
	
}

void Dict::show() 
{
	this->bplus->show();
}


void Dict::serialize(std::string file) 
{
	this->trie->serialize(file);
}

void Dict::deserialize(std::string file) 
{
	this->trie->deserialize(file);
}
