/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "trie.cpp"
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

#define DEBUG true
#define DEFAULT_OUT "tests/output.trie"

#include "trie.h"
#include <iomanip>


/* Function will be used as a constructor for Trie where the input is de-serialized*/
Trie::Trie()
{
	this->root = new Trie::Node();
	this->save_file = DEFAULT_OUT;
}

/* Function will be used as a constructor for Trie where the input is de-serialized*/
Trie::Trie(std::string ofile)
{
	this->root = new Trie::Node();
	this->save_file = ofile;
}

/* Function will be used as a constructor for Trie where the input is de-serialized*/
Trie::Trie(std::string ifile, std::string ofile)
{
	this->root = new Trie::Node();
	this->save_file = ofile;
	if (this->fileExists(ifile))
		this->deserialize(ifile);
}

/* Function will be Trie Destructor where it serializes a file and then clears the original file*/
Trie::~Trie() 
{
	if (this->save_file.length())
		this->serialize(this->save_file);
	this->clear();
}

// Simple utility to chech if a file exists
bool Trie::fileExists(std::string path)
{
	std::ifstream ifs;
	ifs.open(path);
	if (!ifs)
	{
		return false;
	}
	ifs.close();
	return true;
}

/* Function will traverse to form the Trie tree */
void Trie::traverse(Trie::Node *N, std::string s) 
{
	if (N == nullptr) return;
	std::set<Trie::Node *>::iterator I = N->children.begin();
	
	std::cout << N->letter << std::endl;
	if (N->isWord) 
		std::cout << N->id << " : " << s << std::endl;
	
	while (I != N->children.end()) 
	{
		std::string _s = s;
		_s.push_back((*I)->letter);
		traverse(*I, _s);
		++I;
	}
}

/* Function will go through the trie tree and delete the children recursively*/
void Trie::clear_recursive(Trie::Node *N) 
{
	if (N == nullptr) return;
	typedef typename std::set<Trie::Node *>::iterator itr;
	for (itr I = N->children.begin(); I != N->children.end(); ++I)
		clear_recursive(*I);
	N->children.clear();
}

/* Function will start at root within the header of Trie tree and go through 
the children to find a match. If no match, return Null; otherwise, return N, 
which is a Node*/
Trie::Node *Trie::find(std::string s) 
{
	Trie::Node *N = this->root;
	if (N == nullptr) return nullptr;
	std::set<Trie::Node *>::iterator I = N->children.begin();
	bool match = 0;

	if (s.length() < 1) 
		return N;

	for (char &c: s) 
	{
		while (I != N->children.end()) 
		{
			match = (c == (*I)->letter);
			if (match) 
			{
				N = *I;
				I = N->children.begin();
				break;
			}
			++I;			
		}
		if (!match) 
			return nullptr;
		match = 0;
	}
	
	return N;
}

/* Function will use  the Trie tree to insert into it. If there is another node
same as the about to be inserted node, then that specific child corresponds to 
the next character in the string that we are trying to insert. If no match in 
any of the children, then insert at that node's children. When you are at the 
final node, set the node to indicate that it corresponds to a string in the 
Trie with a given ID. Return Node. */
Trie::Node *Trie::insert(std::string s, unsigned int id) 
{
	Trie::Node *N = this->root;
	if (N == nullptr)
	{
		return nullptr;
	}
	std::set<Trie::Node *>::iterator I = N->children.begin();
	bool match = 0;
	int idx = 0;
	
	if (s.length() < 1) 
		return N;
	
	for (char &c: s) 
	{
		while (I != N->children.end()) 
		{
			match = (c == (*I)->letter);
			if (match) 
			{
				N = *I;
				I = N->children.begin();
				break;
			}
			++I;			
		}
		
		if (!match) 
		{
			// here's where insert happens
			for (char &_c : s.substr(idx)) 
			{
				Trie::Node *_N = new Trie::Node(N, _c);
				N->children.insert(_N);
				N = _N;
			}
			break;
		}
		
		match = 0;
		++idx;
	}
	
	N->isWord = true;
	N->id = id;
	
	return N;
}

/* Function will use  the Trie tree to insert into it. If there is another node
same as the about to be inserted node, then that specific child corresponds to 
the next character in the string that we are trying to insert. If no match in 
any of the children, then insert at that node's children. When you are at the 
final node, set the node to indicate that it corresponds to a string in the 
Trie with a given ID. Return Node. */
void Trie::insert(std::string s) 
 {
 	Trie::Node *N = this->root;
	if (N == nullptr)
	{
		return;
	}
	std::set<Trie::Node *>::iterator I = N->children.begin();
	bool match = 0;
	int idx = 0;
	
	if (s.length() < 1) 
		return;
	
	for (char &c: s) 
	{
		while (I != N->children.end()) 
		{
			match = (c == (*I)->letter);
			if (match) 
			{
				N = *I;
				I = N->children.begin();
				break;
			}
			++I;			
		}
		
		if (!match) 
		{
			// here's where insert happens
			for (char &_c : s.substr(idx)) 
			{
				Trie::Node *_N = new Trie::Node(N, _c);
				N->children.insert(_N);
				N = _N;
			}
			break;
		}
		
		match = 0;
		++idx;
	}
	
	N->isWord = true;	
	N->id = 0;	
 }

 /* Function finds the string. Then it will check to see if the node is empty 
 or is the root. If so, return. Otherwise, set it being not a word. If there is
 not children, For the  not root node and the parent node of root, if no
 children and not a world, then remove the parent node and put the root node 
 as the parent node. */
void Trie::remove(std::string s) 
{
	Trie::Node *N = this->find(s), *_N;
	
	if (N == nullptr || N == this->root) 
		return;
	
	N->isWord = false;
	
	if (N->children.empty()) 
	{
		for (_N = N->parent; N != this->root; _N = N->parent) 
		{
			if (N->children.empty() && !N->isWord) 
			{
				_N->children.erase(N);
				N = _N;				
			}
			else 
				break;
		}
	}
}
 
 /* Function finds the string. Then it will check to see if the node is empty
 or is the root. If so, return. Return false by default */
bool Trie::contains(std::string s) 
{
	Trie::Node *N = find(s);
	if (N != nullptr && N != this->root)
	{
		if (N->isWord) 
		{
			return true;
		}
	}
	return false;
}

 /* Function will check if the node is null or not a word; if so, return "". IF
 the node is not a root, reverse the letter until you hit the parent. Finally 
 reverse the letters and return the string.*/
std::string Trie::getString(Node * N) 
{
	std::string reverse = "";
	
	if (N == nullptr)
	{
		return reverse;
	}
	if (!N->isWord) 
	{
		return reverse;
	}
	
	while (N != this->root) 
	{
		if (N == nullptr)
		{
			std::cerr << "Fatal Error: Broken link to parent in Trie\n";
			exit(-1);
		}
		reverse += N->letter;
		N = N->parent;
	}
	
	for (unsigned int i = 0; i < 1 + reverse.length() / 2; ++i)
		std::swap(reverse[i], reverse[reverse.length() - 1 - i]);
	
	return reverse;
}

 /* Function will recursively clear the root and it's children*/
void Trie::clear() 
{
	clear_recursive(this->root);
}

 /* Function will use traverse to traverse through the root*/
void Trie::show() 
{
	traverse(this->root, "");
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

/////////////// FUNCTIONS BELOW ARE USED FOR SERIALIZTION /////////////////////////

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


 /* Function will open a file and write the root info into the file in binary*/
void Trie::serialize(std::string ofile) 
{
	this->trie_ostream.open(ofile, std::ios::out | std::ios::binary);
	writeNode(this->root);	
}


 /* Function will write data as well as the size of the data to the file*/
template <typename T>
void Trie::writeData(std::ofstream& ofs, T data) 
{
	ofs.write(reinterpret_cast<const char *>(&data), sizeof(data));
}

 /* Function will read the data and the size of the data*/
template <typename T>
void Trie::readData(std::ifstream& ifs, T *data) 
{
	ifs.read(reinterpret_cast<char *>(data), sizeof(*data));
}

 /* Function will write in the file nodes id, letter, word, and children and 
 children's children*/
void Trie::writeNode(Node *N) 
{
	if (N == nullptr) 
	{
		return;
	}
	unsigned int nChildren = N->children.size();
	
	writeData<unsigned int>(this->trie_ostream, N->id);
	writeData<char>(this->trie_ostream, N->letter);
	writeData<bool>(this->trie_ostream, N->isWord);
	writeData<unsigned int>(this->trie_ostream, nChildren);
	
	for (auto child : N->children)
		writeNode(child);
}

 /* Function will open the file and read the information from the file in 
 binary*/
void Trie::deserialize(std::string ifile)
{
	return;
	// first clear the current tree and then reset the root
	this->clear();
	if (this->root == nullptr)
	{
		this->root = new Node();
	}

	this->trie_istream.open(ifile, std::ios::in | std::ios::binary);
	readNode(this->root);
	this->trie_istream.close();
}


 /* Function will read the node from the file and reads in the nodes ID, 
 letter, word, and children. After the information is read, that particular
 node is deleted. This continues until all the nodes are read in. */
void Trie::readNode(Node *N)
{
	// This will tell us how many children N should obtain
	unsigned int _numChildren;
	
	// This follows the same exact pattern as the writeNode functionality
	readData<unsigned int>(this->trie_istream, &N->id);
	readData<char>(this->trie_istream, &N->letter);
	readData<bool>(this->trie_istream, &N->isWord);
	readData<unsigned int>(this->trie_istream, &_numChildren);

	// For now this is simple proof that Nodes are being read properly
	std::cout << std::setw(10) << N->id << " | " 
		<< N->letter << " | " 
		<< (N->isWord ? "*" : " ") << " | " 
		<< _numChildren << "\n";
	
	// Accumulate children according to the count
	for (unsigned int i = 0; i < _numChildren; ++i)
	{
		// create a new child node and assign N as its parent
		Node *C = new Node(N);
		N->children.insert(C);
		
		// recurse into the child
		readNode(C);
	}

}
