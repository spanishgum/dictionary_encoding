#include <iostream>
#include "bplus.h"
#include "trie.h"

using namespace std;

int main() {
	Trie *T = new Trie();
	Bplus<int, int> *B = new Bplus<int, int>();
	int testKeys[10];
	int testDats[10];
	for (int i = 0; i < 10; ++i)
		testKeys[i] = testDats[i] = i*3 % 20;
	for (int i = 0; i < 10; ++i)
		B->insert(testKeys[i], testDats[i]);
	return 0;
}