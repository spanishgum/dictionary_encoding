#ifndef _RDF2BTD_H
#define _RDF2BTD_H

#include "dictionary.h"

#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <algorithm>



class rdf2btd 
{
	

	// The following struct provides constant indices
	//  into the above tuple types.
	struct CompressedTriple
	{
		static const unsigned int S() { return 0; }
		static const unsigned int P() { return 1; }
		static const unsigned int O() { return 2; }
		
		unsigned int data[3];
		
		unsigned int &get(const unsigned int i)
		{
			if (i < 3)
			{
				return data[i];
			}
			else
			{
				std::string msg = "Invalid request for CompressedTriple member";
				fatal_err(msg, 1);
			}
			return data[0]; // shuts up compiler
		}
		
		unsigned int *getptr(const unsigned int i)
		{
			if (i < 3)
			{
				return &data[i];
			}
			else
			{
				std::string msg = "Invalid request for CompressedTriple member";
				fatal_err(msg, 1);
			}
			return NULL; // dont need to shut up compiler :)
		}
		
	};
	
	struct TripleCompressor 
	{
		rdf2btd &instance;
		
		inline TripleCompressor(rdf2btd &r2d)
			: instance(r2d) { }
			
		unsigned int operator() (std::string s)
		{
			return this->instance.dict->insert(s);
		}
	};
	
	private:
	
		
		Dict *dict;
		std::string ifile, tfile, ofile;
		std::string n3parser;
		std::vector<CompressedTriple> ctriples;


		static void fatal_err(std::string, int);
		
		void verify_file(std::ifstream &ifs, std::string msg);
		
		void transform(std::string, std::string);
		void compress(std::string);
		
		template <typename T>
		void writeData(std::ofstream& ofs, T data);
		
		template <typename T>
		void readData(std::ifstream& ifs, T *data);
		
	public:
		rdf2btd(std::string, std::string);
		~rdf2btd();

		void run();
		void save(std::string);
		void load(std::string);
		
		
		

};


#endif
