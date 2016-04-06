#include <iostream>
#include "bplus.h"
#include "trie.h"

using namespace std;

int main() {
	// Trie *T = new Trie();
	Bplus<int, void*> *B = new Bplus<int, void*>();
	B->insert(0, NULL);
	B->insert(0, NULL);
	B->insert(1, NULL);
	
	
	return 0;
}