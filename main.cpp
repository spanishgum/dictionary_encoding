/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "main.cpp"
*
* This project explores the idea of creating a 2-way hash map to promote:
*   2-way search performance
*   data compression
*   low memory overhead
*
* Large datasets from the DBpedia are be used to challenge our software
*   so that results can be compared to that of HDT
*
* While this project certainly met some level of the goals above,
*   HDT is a much more mature software solution which employs 
*   a series of advances techniques which were beyond the scope of this work.
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <string.h>

#include "rdf2btd.h"

// Dont worry about polluting the namespace here
using namespace std;

// timer function to measure execution times
void timeit(void(*)(void));

// path utility functions to double check 
//  cmd line argument files
bool fileExists(string path);

// wrapper that handles extracting lines from
//  a file given its path
vector<string> getLinesFrom(string path);

// prototypes to run mini quick tests
void runQuickTest_D(string path);
void runQuickTest(string path);


// print simple usage error to screen
static void usage()
{
	cerr << "Usage: ./main [-h] [-d] [-t0] [-t1] [-i infile -o outfile]\n";
	exit(0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	std::string ifile;
	std::string ofile;
	bool iflag = 0, oflag = 0;
	
	// arguments are a must
	if (argc < 1) 
	{
		usage();
	}
	
	// scan the command line flags to determine 
	// the execution strategy
	for (int i = 1; i < argc; ++i)
	{
		//////////////////////////////////////
		//
		// help message flag to get usage
		//
		//////////////////////////////////////
		if (strcmp(argv[i], "-h") == 0) 
		{
			usage();
		}
		//////////////////////////////////////
		//
		// Here are a few provided test cases
		//
		//////////////////////////////////////
		else if (strcmp(argv[i], "-d") == 0)
		{
			runQuickTest_D("tests/linux_dict");
		}
		else if (strcmp(argv[i], "-t0") == 0)
		{
			runQuickTest("tests/nTriples.nt");
		}
		else if (strcmp(argv[i], "-t1") == 0)
		{
			ifile = "tests/nTriples.nt";
			ofile = "tests/nTriples.btd";
			iflag = oflag = 1;
			break;
		}
		//////////////////////////////////////
		//
		// If no test case is called, 
		//  input and output flags must be 
		//  given otherwise exit
		//
		//////////////////////////////////////
		else if (strcmp(argv[i], "-i") == 0)
		{
			if (++i < argc)
			{
				iflag = 1;
				ifile = argv[i];
			} 
			else usage();
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (++i < argc)
			{
				oflag = 1;
				ofile = argv[i];
			} 
			else usage();
		}
		else
		{
			cerr << "Warning: Ignoring flag " << argv[i]
				<< endl;
		}
	}

	// If we are here, then the standard usage [-i in -o out]
	//  is assumed. Ensure that file arguments were provided
	if (!(iflag && oflag))
	{
		cerr << "Missing file arguments\n";
		exit(0);
	}
	else if (!fileExists(ifile))
	{
		// Dont bother trying to run rdfbtd on bad file paths
		//  (Even though it should handle it in theory)
		exit(0);
	}
	
	// We have both -i and -o files and will run the converter over it
	std::cerr << "\n\nBuilding rdf2btd converter" 
		<< "\n\tinput: " << ifile << "\n\toutput: " << ofile 
		<< "\n\n";
	
	// Build the core program and launch it
	rdf2btd Converter(ifile, ofile);
	Converter.run();
	
	
	std::cout << "\nSuccess!\n";
	return 0;
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

// Wrapper that extracts all lines from a file
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

// First test case which simply tries to build
//  a dictionary with a file of words 
void runQuickTest_D(string path)
{	
	if (!fileExists(path)) 
	{
		exit(-1);
	}
	
	Dict *D = new Dict();
	vector<string> dat = getLinesFrom(path);
	vector<unsigned int> ids;
	
	cout << "Test case for the linux dictionary file\n"
		<< "(contains about 50000 words)\n\n";
	
	// Populate the dictionary
	for (auto str : dat)
	{
		ids.push_back(D->insert(str));
	}
	
	cout << "Dictionary population successful\n";
	
	// output a sensible fraction of results
	for (unsigned int i = 1; i < dat.size(); ++i)
	{
		if (rand() % 500 == 0)
		{
			// test that loc / ext correspond to each other
			string &str = dat[i];
			cout << "loc: " << D->locate(str) << " : " << str << endl;
			cout << "ext: " << i << " : " << D->extract(i) << endl;
			cout << endl;
		}
	}
	
	cout << "Testing serialization\n"
		<< "Destination: tests/serialize.test\n\n";
	
	// Attempt serialization
	D->serialize("tests/serialize.test");
	cout << "Done\n";
	
	cout << "Testing DEserialization\n"
		<< "Source: tests/serialize.test\n\n";
	
	// Attempt deserialization
	D->deserialize("tests/serialize.test");
	cout << "Done.\n";
}

// template for tests
void runQuickTest(string ifile)
{
	string ofile = ifile + ".out";
	// Build the core program and launch it
	rdf2btd Converter(ifile, ofile);
	Converter.run();	
	
	exit(0);
}

