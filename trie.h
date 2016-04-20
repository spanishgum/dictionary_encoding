#ifndef TRIE_H
#define TRIE_H

#include <cstdlib>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

class Trie 
{
	public:
	
		struct Node 
		{
			Node *parent;
			std::list<Node *> children;
			char letter;
			bool isWord;
			unsigned int id;
			
			inline Node(Node *p = NULL, char c = '\0')
				: parent(p), letter(c), isWord(0) { }
		};

		Node *root;
		std::string save_file;
		
		void clear_recursive(Node *);
		void traverse(Node *, std::string);

		Node *find(std::string);
		Node *insert(std::string, unsigned int);

		template <typename T>
		void writeData(std::ofstream &, T);

		template <typename T>
		void readData(std::ifstream &, T*);

		void writeNode(std::ofstream&, Node *N);
		void readNode(std::ifstream&);

	public:
	
		Trie(std::string = "", std::string = "");
		~Trie();
		
		void insert(std::string);
		void remove(std::string);

		std::string getString(Node *);
		bool contains(std::string);
		
		void clear();
		void show();

		void serialize(std::string);
		void serialize(std::ofstream&);
		void deserialize(std::string);
		void deserialize(std::ifstream&);
};


#endif
