#include <iostream>
#include <fstream>
#include "dictionary.h"
#include <vector>
using namespace std;

#define TESTMAX 10000

int testPopulationOf(Dict *& D);
int testSerializationOf(Dict *& D);

int main() 
{

	std::string ifile = "/usr/share/dict/cracklib-small";
	std::string ofile = "testOut.dict";
	Dict *D = new Dict(ifile, ofile);
	
	testPopulationOf(D);
	testSerializationOf(D);
	
	return 0;
}

int testPopulationOf(Dict *& D, std::string testFileName = "/usr/share/dict/cracklib-small")
{
	ifstream ifs;

	std::cout << "Testing population of Dictionary object using " 
		<< testFileName << "\n";
	ifs.open(testFileName);
	if (!ifs) 
	{
		std::cerr << "Error: " << testFileName << " does not exist\n"
		return -1;
	}
	
	vector<unsigned int> V;
	std::string line;
	while (getline(ifs, line)) {
		V.push_back(D->insert(line));
	}
	ifs.close();
	
	return 0;
}

int testSerializationOf(Dict *& D)
{
	std::cout << "Beginning test serialization\n";
	D->serialize("serialize.test");
	
	std::cout << "Beginning test deserializtion\n";
	D->deserialize("serialize.test");
	
	return 0;
}
