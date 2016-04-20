/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "trie.h"
*
*     This source file
*
*
*
*
*
*
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <string.h>

#include "rdf2btd.h"

using namespace std;

// timer functions to measuer exceution times
void timeit(void(Dict::*)(void), Dict&);
void timeit(void(*)(void));

// path utility functions to check cmd line arg files
bool fileExists(string path);
vector<string> getLinesFrom(string path);

// prototypes to run quick tests
void runQuickTest_Dict(string path);

// print simple usage error to screen
void usage_err()
{
	cerr << "Usage: ./main [-h] [-d] [-i infile -o outfile]\n";
	exit(0);
}

int main(int argc, char **argv)
{
	std::string ifile = "../nTriples.nt";
	std::string ofile = "../nTriples.btd";
	bool iflag = 0, oflag = 0;
	
	if (argc < 1) 
	{
		usage_err();
	}
	
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "-d") == 0) 
		{
			runQuickTest_Dict("/usr/share/dict/cracklib-small");
		}
		else if (strcmp(argv[i], "-h") == 0) 
		{
			usage_err();
		}
		else if (strcmp(argv[i], "-i") == 0)
		{
			if (++i < argc)
			{
				iflag = 1;
				ifile = argv[i];
			} 
			else usage_err();
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (++i < argc)
			{
				oflag = 1;
				ofile = argv[i];
			} 
			else usage_err();
		}
	}

	if (!(iflag && oflag))
	{
		cerr << "Please specify both input and output files. . .\nExiting.\n";
		exit(0);
	}
	else if (!fileExists(ifile))
	{
		exit(0);
	}
	
	
	// We have both -i and -o files and will run the converter over it
	std::cout << "Building rdf2btd converter\n";
	rdf2btd Converter(ifile, ofile);
	Converter.run();
	
	return 0;
}


// Utility for timing
void timeit(void(Dict::*fptr)(void), Dict &D)
{
	clock_t beg = clock();
	(D.*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

// Utility for timing
void timeit(void(*fptr)(void))
{
	clock_t beg = clock();
	(*fptr)();
	clock_t res = clock() - beg;
	float result = ((float)res) / CLOCKS_PER_SEC;
	std::cout << "\n" << result << "s\n";
}

// Utility for checking file exists
bool fileExists(string path)
{
	ifstream ifs;
	ifs.open(path);
	if (!ifs) 
	{
		std::cerr << "Error: Invalid path: " 
			<< path << endl;
		return false;
	}
	ifs.close();
	return true;
}

// Utility for checking file exists
vector<string> getLinesFrom(string path)
{
	string line;
	ifstream ifs;
	vector<string> lines;
	if (!fileExists(path)) return lines;
	ifs.open(path);
	while (getline(ifs, line))
	{
		lines.push_back(line);
	}
	ifs.close();
	return lines;
}


void runQuickTest_Dict(string path)
{
	unsigned int i = 0;
	if (!fileExists(path)) return;
	Dict *D = new Dict();
	vector<string> dat = getLinesFrom(path);
	vector<unsigned int> ids;
	cout << "Beginning simple dictionary object test . . .\nLoading . . .\n";
	for (auto str : dat)
	{
		ids.push_back(D->insert(str));
	}
	cout << "Testing Locate(string) and Extract(id) for simple dictionary . . .\n";
	for (auto str : dat)
	{
		if (rand() % (int) (1 + (dat.size() / 1000)))
		{
			cout << "Locate(" << str << "): " << D->locate(str) << endl;
			cout << "Extract(" << i << "): " << D->extract(i) << endl;
			if (!D->extract(i).compare(str))
			{
				cout << "\tGOOD\n";
			}
			else
			{
				cout << "\tBAD\n";
			}
		}
		++i;
	}
	cout << "Testing dictionary serialization to tests/serialize.test\n";
	D->serialize("tests/serialize.test");
	cout << "Done\n\nTesting dictionary deserialization from tests/serialize.test\n";
	D->deserialize("tests/serialize.test");
	cout << "Done.\n";
}

