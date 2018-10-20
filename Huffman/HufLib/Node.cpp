#include "Node.h"


Node::Node()  {
	left = nullptr;
	right = nullptr;
	v = nullptr;
}

Node::Node(unsigned char value, uint64_t weight) : value(value), weight(weight) {
	left = nullptr;
	right = nullptr;
}

Node::Node(uint64_t weight, Node &left, Node &right) : 
	weight(weight), left(&left), right(&right) {}

Node::~Node() {
	if (left != nullptr) delete left;
	if (right != nullptr) delete right;
}

Node::Node(Node *v) : v(v) {
	left = nullptr;
	right = nullptr;
}

FilestreamCorruptedException::FilestreamCorruptedException() = default;
FileCorruptedException::FileCorruptedException() = default;
TreeCorruptedException::TreeCorruptedException() = default;
