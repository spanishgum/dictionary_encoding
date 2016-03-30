#include "trie.h"

Trie::Trie() {
	root = new Trie::Node();
}

Trie::~Trie() {
	this->clear();
}

void Trie::traverse(Trie::Node *N, std::string s) {
	std::list<Trie::Node *>::iterator I = N->children.begin();
	std::cout << N->letter << std::endl;
	if (N->isWord) std::cout << N->id << " : " << s << std::endl;
	while (I != N->children.end()) {
		std::string _s = s;
		_s.push_back((*I)->letter);
		traverse(*I, _s);
		++I;
	}
}

void Trie::clear_recursive(Trie::Node *N) {
	typedef typename std::list<Trie::Node *>::iterator itr;
	for (itr I = N->children.begin(); I != N->children.end(); ++I)
		clear_recursive(*I);
	delete N;
}

Trie::Node *Trie::find(std::string s) {
	Trie::Node *N = this->root;
	std::list<Trie::Node *>::iterator I = N->children.begin();
	bool match = 0;
	if (s.length() < 1) return N;
	for (char &c: s) {
		while (I != N->children.end()) {
			match = (c == (*I)->letter);
			if (match) {
				N = *I;
				I = N->children.begin();
				break;
			}
			++I;			
		}
		if (!match) return NULL;
		match = 0;
	}	
	return N;
}

Trie::Node *Trie::insert(std::string s, unsigned int id) {
	Trie::Node *N = this->root;
	std::list<Trie::Node *>::iterator I = N->children.begin();
	bool match = 0;
	int idx = 0;
	if (s.length() < 1) return N;
	for (char &c: s) {
		while (I != N->children.end()) {
			match = (c == (*I)->letter);
			if (match) {
				N = *I;
				I = N->children.begin();
				break;
			}
			++I;			
		}
		if (!match) {
			// heres where insert happens
			for (char &_c : s.substr(idx)) {
				Trie::Node *_N = new Trie::Node(N, _c);
				N->children.push_back(_N);
				N = _N;
			}
			break;
		}
		match = 0;
		++idx;
	}
	N->isWord = true;
	N->id = id;
	return N;
}

void Trie::insert(std::string s) {
	this->insert(s, 0);
}

void Trie::remove(std::string s) {
	Trie::Node *N = this->find(s), *_N;
	if (N == NULL || N == this->root) return;
	N->isWord = false;
	if (N->children.empty()) {
		for (_N = N->parent; N != this->root; _N = N->parent) {
			if (N->children.empty() && !N->isWord) {
				_N->children.remove(N);
				N = _N;				
			}
			else break;
		}
	}
}

bool Trie::contains(std::string s) {
	Trie::Node *N = find(s);
	if (N != NULL && N != this->root)
		if (N->isWord) return true;
	return false;
}

void Trie::clear() {
	clear_recursive(this->root);
}

void Trie::show() {
	traverse(this->root, "");
}