#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <unistd.h>

#include "rdf2btd.h"

using namespace std;

void timeit(void(Dict::*)(void), Dict&);
void timeit(void(*)(void));

int testPopulationOf(Dict *& D, std::string = "/usr/share/dict/cracklib-small");
int testSerializationOf(Dict *& D);


int main(int argc, char **argv)
{
	// std::string ifile = "/usr/share/dict/cracklib-small";
	// std::string ofile = "testOut.dict";
	
	std::string ifile = "../nTriples.nt";
	std::string ofile = "../nTriples.btd";
	
	std::cout << "Building rdf2btd converter\n";
	rdf2btd Converter(ifile, ofile);
	Converter.run();
	
	
	
	// int opt;
	// string cmd_opts = "i:o:vh";
	// while ((opt = getopt(argc, argv, cmd_opts.c_str())) != -1)
	// {
		// switch (opt)
		// {
			// case 'i':
				// std::cout << "Input: " << "...\n";
				// break;
			// case 'o':
				// break;
			// case 'v':
				// break;
			// case 'h':
				// std::cout << "Help menu\n";
				// break;
			// case '?':
			// default:
				// std::cerr << "Command option " << (char) opt 
					// << " not supported\n";
				// break;
		// }
	// }

	
	
	// Dict *D = new Dict(ifile, ofile);
	
	// testPopulationOf(D);
	// testSerializationOf(D);
	
	return 0;
}

void timeit(void(Dict::*fptr)(void), Dict &D)
{
	clock_t beg = clock();
	(D.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

void timeit(void(*fptr)(void))
{
	clock_t beg = clock();
	(*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

int testPopulationOf(Dict *& D, std::string testFileName)
{
	ifstream ifs;

	std::cout << "Testing population of Dictionary object using " 
		<< testFileName << "\n";
	ifs.open(testFileName);
	if (!ifs) 
	{
		std::cerr << "Error: " << testFileName << " does not exist\n";
		return -1;
	}
	
	vector<unsigned int> V;
	std::string line;
	while (getline(ifs, line)) 
	{
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
