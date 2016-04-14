#include <iostream>
#include <fstream>
#include "dictionary.h"
#include <vector>
using namespace std;

#define TESTMAX 10000

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
	
	vector<unsigned int> V;
	string line, testFileName = "/usr/share/dict/cracklib-small";
	ifstream ifs;
	ifs.open(testFileName);
	int i = 0;
	while (getline(ifs, line)) {
		// std::cout << line << '\n';
		V.push_back(D->insert(line));
		// D->show();
		// cin.get();
		// if (++i > TESTMAX) break;
	}
	ifs.close();
	std::cout << "Beginning test serialization\n";
	// ifs.open(testFileName);
	// int idx = 0, test_idx;
	// bool test_result = 1;
	// while (getline(ifs, line)) {
		// test_idx = D->locate(line);
		// test_result &= (test_idx == idx);
		// ++idx;
	// }
	// cout << "Test result: " << test_result << "\n";

	D->serialize("serialize.test");
	std::cout << "Beginning test deserializtion\n";
	D->deserialize("serialize.test");

	
	// unsigned int hello = D->insert("hello");
	// unsigned int world = D->insert("world");
	// cout << hello << " " << world << endl;
	// cout << D->extract(hello) << endl;
	// cout << D->extract(world) << endl;
	
	return 0;
}