#include <iostream>
#include <fstream>
#include "dictionary.h"

using namespace std;

int main() {
	// Trie *T = new Trie();
	// Bplus<int, int> *B = new Bplus<int, int>();
	// int testKeys[10];
	// int testDats[10];
	// for (int i = 0; i < 10; ++i)
		// testKeys[i] = testDats[i] = i*3 % 20;
	// for (int i = 0; i < 10; ++i)
		// B->insert(testKeys[i], testDats[i]);
	
	Dict *D = new Dict();
	
	string line, testFileName = "/usr/share/dict/cracklib-small";
	ifstream ifs;
	ifs.open(testFileName);
	while (getline(ifs, line)) {
		cout << D->insert(line) << "\n";
	}
	
	// unsigned int hello = D->insert("hello");
	// unsigned int world = D->insert("world");
	// cout << hello << " " << world << endl;
	// cout << D->extract(hello) << endl;
	// cout << D->extract(world) << endl;
	
	return 0;
}