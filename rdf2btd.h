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
*     This header file
*
*
*
*
*
*
*
*/

#ifndef _RDF2BTD_H
#define _RDF2BTD_H

#include "dictionary.h"

#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <algorithm>


#define TMP_FILE "tests/rdf2btd_tmp"


class rdf2btd 
{
	
	// enumerate the elements of a triple
	enum tri_idx { Subject, Predicate, Object, End };
	
	// The following struct provides constant indices
	//  into the above tuple types.
	struct CompressedTriple
	{
		static const unsigned int S() { return 0; }
		static const unsigned int P() { return 1; }
		static const unsigned int O() { return 2; }
		
		unsigned int data[3];
		
		// init the array to 0s
		inline CompressedTriple()
			: data{} { }
		
		// get a reference to an element
		unsigned int &get(tri_idx i)
		{
			// tri_idx will always be 0 <= i < 3
			return data[i];
		}
		
		// get a ptr to an element
		unsigned int *getptr(tri_idx i)
		{
			// tri_idx will always be 0 <= i < 3
			return &data[i];
		}
		
		// set an element in data[] by idx
		void set(tri_idx i, unsigned int u)
		{
			data[i] = u;
		}
		
	};
	
	// Syntax sugar functor that makes the nullptr check
	//  for us
	struct TripleCompressor
	{
		// force this to always associate with rdf2btd
		rdf2btd &instance;
		
		inline TripleCompressor(rdf2btd &r2d)
			: instance(r2d) { }
			
		unsigned int operator() (std::string s)
		{
			if (this->instance.dict != nullptr)
			{
				return this->instance.dict->insert(s);
			}
			else
			{
				std::cerr << "Could not compress " << s
					<< " dictionary is invalid.\n";
				return 0;
			}
		}
	};

	
	private:
	
		// internal constant and error wrapper / checker
		static void fatal_err(std::string, int);
		static void verify_file(std::ifstream &ifs, std::string msg);
		std::vector<std::string> getLinesFrom(std::string path);
		
		// the dictionary and several contextual paths
		Dict *dict;
		std::string ifile, ofile, tfile;
		std::string n3parser;
		
		// container to store our compressed triples
		std::vector<CompressedTriple> ctriples;

		// Internal components that make up the conversion process
		void transform(std::string, std::string);
		void compress(std::string);
		
		// simple binary writer wrapper
		template <typename T>
		void writeData(std::ofstream& ofs, T data);
		
		// simple binary reader wrapper
		template <typename T>
		void readData(std::ifstream& ifs, T *data);
	
	protected:
	
		// Empty constructor for a program interface
		rdf2btd();
		
		// public functions usable by a program interface
		//  if more conversions are desired
		void save(std::string);
		void load(std::string);
		
	public:
		
		// Always take an input and output, but default the tmpfile
		rdf2btd(std::string, std::string, std::string = TMP_FILE);
		~rdf2btd();
		
		// cmd to initiate the program
		void run();

};


#endif
