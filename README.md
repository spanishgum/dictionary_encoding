# Dictionary encoding 
## A research project on RDF storage compression
## by Adam Stallard and Aranya Ajith

COP5725 Advanced Databases Project
Spring 2016

### Ready

  > git clone https://github.com/spanishgum/dictionary_encoding


### Set

  > cd path/to/dictionary_encoding
  > make fresh

### Go

  > ./main -i infile -o outfile
  > ./main -d
  > ./main -d2


#### Test cases

 * -i in -o out 
	* Simply link to a '.nt' file for input
	 * It will be transformed to an intermediate file
	 * Then compressed to an output file
 * -d1 is a base test case that uses a linux dictionary file
	* This shows an example of where our data structure performs poorly
	 * The dataset first off is very small
	 * The nature of the file is that words are very dissimilar
 * -d2 is a base test case that works on "tests/nTriples.nt"
	* This shows a case where compression works!
	 * However, triples are not maintained so this is still yet to be determined

 * Functionality for saving Triples is included in the code, but not all bugs are proprly worked out
	 
	 
## Discussion

Typical RDF storage models implement some form of dictionary encoding to map strings to integers, which promotes efficient query processing and compression. Common implementations involve the use of two way hash maps, tree structures, arrays, etc.

This project explores the idea of creating a 2-way hash map to promote:
* 2-way search performance
* data compression
* low memory overhead

A big challenge involved creating a presistent file through serialzation techniques. This project did successfully create a method to persist the Trie data structure essentially through a depth first recursion and storing the data in binary format. This file can be read back in at load time.
* The file that is persisted does not include the original triple structure
* Rather it simply records the dictionary itself, namely the Trie
 * The B+ tree index is essentially reloaded on the fly and joined to create the full dictionary
* Future work may involve benchmarking bulk loading


* We do this by linking a B+ tree at the leaf Nodes, to the tails of a Trie structure.
 * The Trie is a simple implementation which provides public methods for search, insertion and removal of strings.
 * The B+ is a templated implementation which provides public methods for search, insertion and removal of keys.

The dictionary object will encapsulate the Trie and a B+ using unsigned integer keys, and handle linking the two data structures so that a user may call public methods for insertion, removal, and of course the most important methods, Locate and Extract. 

# Locate(string s)
### return a mapped int ID corresponding to s

# Extract(int i)
### return a mapped string corresponding to i


A primary aim of this project is to test and demonstrate compression for large RDF data sets such as DBpedia
* We hypothesis that there are 2 major factors that improve compression rate in this data set
 * Highly connectivity
 * Data set string similarity 
 
It is intuitive that a Trie structure achieves higher compression the closer the string sets are
At the same time, if a graph is highly connected, then much of the original file contains repeated Subjects and Objects, again leading to better Trie compression

This dictionary model is novel, but may come at the sacrifice of query processing
* Tree traversal is slower than hashing
 * Still, generic hashing does not promote compression as the source data is still maintained on disk


