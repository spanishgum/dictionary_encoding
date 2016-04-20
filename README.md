# Dictionary encoding 
## and a comparison of RDF storage compression with HDT
## by Adam Stallard and Aranya Ajith

COP5725 Advanced Databases Project
Spring 2016

Typical RDF storage models implement some form of dictionary encoding to map strings to integers, which promotes efficient query processing and compression. Common implementations involve the use of two way hash maps, tree structures, arrays, etc.

This project explores the idea of creating a 2-way hash map which allows for fair 2-way search performance, and efficient data compression in both memory and when serialized to disk.

* We do this by linking a B+ tree at the leaf Nodes, to the tails of a Trie structure.
 * The Trie is a simple implementation which provides public methods for search, insertion and removal of strings.
 * The B+ is a templated implementation which provides public methods for search, insertion and removal of keys.

The dictionary object will encapsulate the Trie and a B+ using unsigned integer keys, and handle linking the two data structures so that a user may call public methods for insertion, removal, and of course the most important methods, Locate and Extract. Locate and Extract are used to recover the mapped key for a string and vice versa.

The primary aim of this project is to demonstrate large compression for massive RDF data sets such as DBpedia, which will fit fully in memory. This will likely come at the sacrifice of query processing, as a tree traversal has higher complexity than existing methods which use array access through integer keys. However, this model supports dynamic storage.

