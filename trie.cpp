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

// void Trie::insert(std::string s) {
	// this->insert(s, 0);
// }

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

std::string Trie::getString(Node * N) {
	if (N == NULL) return "";
	if (!N->isWord) return "";
	std::string reverse = "";
	while (N != this->root) {
		reverse += N->letter;
		N = N->parent;
	}
	for (unsigned int i = 0; i < 1 + reverse.length() / 2; ++i)
		std::swap(reverse[i], reverse[reverse.length() - 1 - i]);
	return reverse;
}

void Trie::clear() {
	clear_recursive(this->root);
}

void Trie::show() {
	traverse(this->root, "");
}

void Trie::serialize(std::string ofile) {
	std::ofstream ofs;
	ofs.open(ofile, std::ios::binary);
	writeNode(ofs, this->root);	
}

template <typename T>
void Trie::writeData(std::ofstream& ofs, T data) {
	ofs.write(reinterpret_cast<const char *>(&data), sizeof(data));
}
// template void Trie::writeData<char>;
// template void Trie::writeData<bool>;
// template void Trie::writeData<int>;
// template void Trie::writeData<unsigned int>;

template <typename T>
void Trie::readData(std::ifstream& ifs, T *data) {
	ifs.read(reinterpret_cast<char *>(data), sizeof(*data));
}
// template void Trie::readData<char>;
// template void Trie::readData<bool>;
// template void Trie::readData<int>;
// template void Trie::readData<unsigned int>;

void Trie::writeNode(std::ofstream& ofs, Node *N) {
	// ofs << N->id << N->letter << N->isWord << N->children.size();
	writeData(ofs, N->id);
	writeData(ofs, N->letter);
	writeData(ofs, N->isWord);
	char nChildren = (char) N->children.size();
	writeData(ofs, nChildren);
	
	std::cout << N->id << "|" << N->letter << "|" << N->isWord << "|" << N->children.size()<< "\n";
	for (auto child : N->children)
		writeNode(ofs, child);
}

void Trie::deserialize(std::string ifile) {
	std::ifstream ifs;
	ifs.open(ifile, std::ios::binary);
	readNode(ifs);
}

void Trie::readNode(std::ifstream& ifs) {

	// ifs.read(reinterpret_cast<char *>(&_id), sizeof(_id));
	// ifs.read(reinterpret_cast<char *>(&_letter), sizeof(_letter));
	// ifs.read(reinterpret_cast<char *>(&_isWord), sizeof(_isWord));
	// ifs.read(reinterpret_cast<char *>(&_numChildren), sizeof(_numChildren));
	do {
		Node *N = new Node();
		readData(ifs, &N->id);
		readData(ifs, &N->letter);
		readData(ifs, &N->isWord);
		char _numChildren;
		readData(ifs, &_numChildren);

		std::cout << N->id << "|" << N->letter << "|" << N->isWord << "|" << (unsigned int) _numChildren << "\n";
		delete N;
	} while (ifs);
}



