#include <iostream>
#include <string>
#include <cstdlib>

#include "trie.h"
#include "bplus.h"

class Dict : public Trie, public Bplus<int, Trie::Node> {
	typedef Trie::Node TNode;
	typedef Bplus::Node BNode;

	private:
		Trie trie;
		Bplus<int, Trie::Node> bplus;
		int size;

		void insert(std::string, int);
	public:
		Dict();
		~Dict();
		void insert(std::string);
		void remove(std::string);
		void remove(int);
		int locate(std::string);
		std::string extract(int);
		void clear();
		void show();
};
