#ifndef HUFFMAN_LIBRARY_H
#define HUFFMAN_LIBRARY_H

#include <fstream>

static constexpr size_t SIZE_OF_ALPHABET = 256;
static constexpr size_t SIZE_OF_BUFFER = 1024 * 1024;

struct Node {
	//parts
	unsigned char value;
	uint64_t weight;
	Node* left;
	Node* right;
	Node* v;

	//constructor
	Node();
	Node(uint64_t weight, Node &left, Node &righ);
	Node(unsigned char value, uint64_t weight);
	~Node();

	//destructor
	explicit Node(Node *v);
};

//comparator

//exeptions
struct FilestreamCorruptedException { FilestreamCorruptedException(); };
struct FileCorruptedException { FileCorruptedException(); };
struct TreeCorruptedException { TreeCorruptedException(); };

#endif
