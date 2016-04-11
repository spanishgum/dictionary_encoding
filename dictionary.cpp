#include "dictionary.h"


Dict::Dict() {
	this->size = 0;
}

Dict::~Dict() {

}

void Dict::insert(std::string s, unsigned int id) {
	TNode *tn = this->trie.insert(s, id);
	BNode *bn = this->bplus.insert(id, *tn);
}

unsigned int Dict::insert(std::string s) {
	this->insert(s, this->size);
	return this->size++;
}

void Dict::remove(std::string) {

}

void Dict::remove(unsigned int) {

}

int Dict::locate(std::string) {

	return 0;
}

std::string Dict::extract(unsigned int id) {
	TNode *tn = this->bplus.find(id);
	return this->trie.getString(tn);
	return "";
}

void Dict::clear() {

}

void Dict::show() {

}


