#include "encrypt.h"
#include "Node.h"

#include <algorithm>
#include <queue>

void encrypter::encrypt(std::string fileIn, std::string fileOut) {
	//try to open file in
	fIn.open(fileIn, std::ios::binary);
	if (!fIn.is_open()) { throw FilestreamCorruptedException(); }

	//fry to open file out
	fOut.open(fileOut, std::ios::binary);
	if (!fOut.is_open()) { fIn.close(); throw FilestreamCorruptedException(); }

	completeAlphabet();

	fOut.write(&convertToCharArray(numberOfSymbols)[0], sizeof(numberOfSymbols));

	//if we haven't enough symbols
	if (numberOfSymbols == 0) { fOut.close(); return; }
	if (numberOfSymbols == 1) { ++numberOfSymbols; }

	//build tree
	treeDescription.resize(roundUp(2 * (numberOfSymbols - 1), 8));
	build();

	std::vector<bool> tmp_code;
	uint16_t zero = 0;
	dfs(tree, tmp_code, zero);
	uint64_t afterEnc = sizeAfterEncrypt();

	fOut.write(&convertToCharArray(afterEnc)[0], sizeof(afterEnc));
	fOut.write(&treeDescription[0], sizeof(char) * treeDescription.size());
	fOut.write(&orderOfLists[0], sizeof(char) * orderOfLists.size());

	char *buffer_in = new char[SIZE_OF_BUFFER];
	char *buffer_out = new char[SIZE_OF_BUFFER];

	fIn.clear();
	fIn.seekg(0, std::ios_base::beg);

	const size_t BUF_SIZE_IN_BITS = SIZE_OF_BUFFER * sizeof(char) * 8;
	size_t indBuf = 0;

	while (!fIn.eof()) {
		fIn.read(buffer_in, SIZE_OF_BUFFER);
		for (size_t i = 0; i < fIn.gcount(); ++i) {
			size_t indexKeeper = encryptedIntoBuffer(codes[ (unsigned char)buffer_in[i] ], 0, buffer_out, indBuf, BUF_SIZE_IN_BITS); 
			if (indBuf == BUF_SIZE_IN_BITS) {
				fOut.write(buffer_out, BUF_SIZE_IN_BITS / 8);
				indBuf = 0;
				encryptedIntoBuffer(codes[(unsigned char)buffer_in[i]], 
					codes[ (unsigned char)buffer_in[i] ].size() - indexKeeper, 
					buffer_out, 
					indBuf, 
					indexKeeper);
			}
		}
	}

	if (indBuf > 0) {
		fOut.write(buffer_out, sizeof(char) * roundUp(indBuf, 8));
	}

	delete[] buffer_in;
	delete[] buffer_out;
	fIn.close();
	fOut.close();
	delete tree;
}
void encrypter::build() { //done
	std::priority_queue< std::pair<int64_t, Node*> > piecesOfTree;
	for (size_t let = 0; let < SIZE_OF_ALPHABET; ++let) {
		if (alphabet[let] != 0) piecesOfTree.push({ -((int64_t)alphabet[let]), new Node(let, alphabet[let]) });
	}
	
	if (piecesOfTree.size() == 1) {
		tree = new Node;
		Node *fake = new Node('\0', UINT64_MAX);
		tree->left = piecesOfTree.top().second;
		tree->right = fake;
		return;
	}
	while (piecesOfTree.size() != 1) {
		Node *left = piecesOfTree.top().second;
		piecesOfTree.pop();
		Node *right = piecesOfTree.top().second;
		piecesOfTree.pop();
		Node *v = new Node((left->weight + right->weight), *left, *right);
		piecesOfTree.push({-(int64_t)(v->weight), v});
	}
	tree = piecesOfTree.top().second;
}

void encrypter::put1(char * data, size_t id) { //done
	data[id / (sizeof(char) * 8)] |=
		(1 <<
		(sizeof(char) * 8 - 1 - (id % (sizeof(char) * 8)))
			);
}

void encrypter::put0(char * data, size_t id) { //done
	data[id / (sizeof(char) * 8)] &= 
		~(1 << 
		(sizeof(char) * 8 - 1 - (id % (sizeof(char) * 8)) )
			);
}

void encrypter::dfs(Node * vers, std::vector<bool> code, uint16_t & step) { //done
	if ((vers->left == nullptr) ^ (vers->right == nullptr)) {
		TreeCorruptedException();
	}
	if (vers->left == nullptr) {
		codes[vers->value] = code;
		orderOfLists.push_back(vers->value);
	} else {
		//to left son
		code.push_back(true);	
		put1(&treeDescription[0], step++);   		
		dfs(vers->left, code, step); 

		//to right son
		code.back() = false;	
		put0(&treeDescription[0], step++);			
		dfs(vers->right, code, step);
	}
}

void encrypter::completeAlphabet() { //done
	numberOfSymbols = 0;
	char *buffer = new char[SIZE_OF_BUFFER];
	while (!fIn.eof()) {
		fIn.read(buffer, SIZE_OF_BUFFER);
		auto sizeOfFIn = fIn.gcount();
		for (size_t i = 0; i < sizeOfFIn; ++i) {
			if (alphabet[(unsigned char)buffer[i]]++ == 0) {
				++numberOfSymbols;
			}
		}
	}
	delete[] buffer;
}

uint64_t encrypter::sizeAfterEncrypt() { //done
	uint64_t sizeAfterEnc = 0;
	for (size_t i = 0; i < SIZE_OF_ALPHABET; ++i)
		sizeAfterEnc += alphabet[i] * codes[i].size();
	return sizeAfterEnc;
}

size_t encrypter::encryptedIntoBuffer(std::vector<bool> const & code, size_t indCode, char * buffer, size_t & indBuf, const size_t BUF_SIZE_IN_BITS) { //done
	size_t indexKeeper = 0;
	for (size_t i = indCode; i < code.size(); ++i) {
		if (indBuf == BUF_SIZE_IN_BITS) {
			indexKeeper = code.size() - i;
			break;
		}
		code[i] ? put1(buffer, indBuf++) : put0(buffer, indBuf++);
	}
	return indexKeeper;
}

vector<char> encrypter::convertToCharArray(uint64_t number) { //done
	size_t size = roundUp(sizeof(uint64_t), sizeof(char));
	std::vector<char> toChar(size);                                
	size_t step = (sizeof(uint64_t) - sizeof(char)) * 8; //byte -> bit                
	for (size_t i = 0; i < size; ++i) {                           
		toChar[i] = 0;											  
		toChar[i] |= number >> step;								  
		step -= sizeof(char) * 8;								  
	}
	return toChar;
}

size_t encrypter::roundUp(size_t toRound, size_t sizeOfBlock) { //done
	return (toRound + (sizeOfBlock - 1)) / sizeOfBlock;
}
