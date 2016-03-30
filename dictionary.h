#include <iostream>
#include <string>
#include <cstdlib>

#include "trie.h"
#include "bplus.h"

class Dict {
	private:
		Trie trie;
		Bplus<int> bplus;
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
