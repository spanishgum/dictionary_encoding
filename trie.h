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

#ifndef TRIE_H
#define TRIE_H

#include <cstdlib>
#include <set>
#include <string>
#include <iostream>
#include <fstream>

class Trie
{
	public:
		struct Node 
		{
			Node *parent;
			char letter;
			bool isWord;
			unsigned int id;
			std::set<Node *> children;
			
			inline Node(Node *p = nullptr, char c = '\0')
				: parent(p), letter(c), isWord(0), id(0), children() 
			{ }
			
		};

		Node *root;
		std::string save_file;
		
		bool fileExists(std::string);
		
		void clear_recursive(Node *);
		void traverse(Node *, std::string);

		Node *find(std::string);
		Node *insert(std::string, unsigned int);

		std::ofstream trie_ostream;
		std::ifstream trie_istream;
		
		template <typename T>
		void writeData(std::ofstream &, T);

		template <typename T>
		void readData(std::ifstream &, T*);

		void writeNode(Node *N);
		void readNode(Node *N);

	public:
	
		Trie();
		Trie(std::string);
		Trie(std::string, std::string);
		~Trie();
		
		void insert(std::string);
		void remove(std::string);

		std::string getString(Node *);
		bool contains(std::string);
		
		void clear();
		void show();

		void serialize(std::string);
		void deserialize(std::string);
};


#endif
