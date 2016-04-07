#ifndef TRIE_H
#define TRIE_H

#include <cstdlib>
#include <list>
#include <string>
#include <iostream>

class Trie {
	public:
		struct Node {
			Node *parent;
			std::list<Node *> children;
			char letter;
			bool isWord;
			void *record_id;
			int id;
			inline Node(Node *p = NULL, char c = '\0') 
				: parent(p), letter(c) {
				isWord = 0;
				record_id = NULL;
			}
		};

		Node *root;
		
		void clear_recursive(Node *);
		void traverse(Node *, std::string);

		Node *find(std::string);
		Node *insert(std::string, unsigned int);

	public:
		Trie();
		~Trie();
		void insert(std::string);
		void remove(std::string);
		bool contains(std::string);
		void clear();
		void show();
};


#endif