#ifndef _RDF2BTD_H
#define _RDF2BTD_H

#include "dictionary.h"
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>

typedef struct 
{
	unsigned int S, O, P;
} compressed_triple;

class rdf2btd 
{
	private:
		Dict *dict;
		std::string ifile, tfile, ofile;
		std::string n3parser;
		std::vector<compressed_triple> ctriples;
		
		
		
	public:
		rdf2btd(std::string, std::string);
		~rdf2btd();

		int transform();
		
		

};


#endif
