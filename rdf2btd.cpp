#include "rdf2btd.h"

rdf2btd::rdf2btd(std::string i, std::string o)
{
	this->ifile = i;
	this->ofile = o;
	this->tfile = "intermediate.tmp";
	this->dict = new Dict();
}

rdf2btd::~rdf2btd()
{
	this->dict->clear();
	delete this->dict;
}


