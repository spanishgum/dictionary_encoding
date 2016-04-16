#include <iostream>
#include <fstream>
#include "dictionary.h"
#include <vector>
using namespace std;

#define TESTMAX 10000

int main() 
{

	std::string ifile = "/usr/share/dict/cracklib-small";
	std::string ofile = "testOut.dict";
	Dict *D = new Dict(ifile, ofile);
	
	testPopulationOf(D);
	testSerializationOf(D);
	
	return 0;
}

int testPopulationOf(Dict *& D) 
{
	
	ifstream ifs;
	std::string testFileName = "/usr/share/dict/cracklib-small";
	ifs.open(testFileName);
	std::cout << "Testing population of Dictionary object using " 
		<< testFileName << "\n";
	
	vector<unsigned int> V;
	std::string line;
	while (getline(ifs, line)) {
		V.push_back(D->insert(line));
	}
	ifs.close();	
}

int testSerializationOf(Dict *& D) 
{
	
	std::cout << "Beginning test serialization\n";
	D->serialize("serialize.test");
	
	std::cout << "Beginning test deserializtion\n";
	D->deserialize("serialize.test");
	
}
