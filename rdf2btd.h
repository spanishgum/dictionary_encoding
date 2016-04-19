#ifndef _RDF2BTD_H
#define _RDF2BTD_H

#include "dictionary.h"


class rdf2btd {
	private:
		Dict *dict;
		std::string ifile, tfile, ofile;
		std::string n3parser;
		

	public:
		rdf2btd(std::string, std::string);
		~rdf2btd();

		

};


#endif
