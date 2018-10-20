#ifndef HUFFMAN_ENCRYPT_H
#define HUFFMAN_ENCRYPT_H

#ifndef HUFFMAN_LIBRARY_H
#include "Node.h"
#endif

#include <vector>

using std::vector;

class encrypter {
public:
	void encrypt(std::string fileIn, std::string fileOut);
private:
	//tree: complete & describe
	void build(); //done
	void put1(char *data, size_t id); //done
	void put0(char *data, size_t id); //done
	void dfs(Node *vers, vector<bool> code, uint16_t &step); //done

	void completeAlphabet(); //done

	//work with encrypted file
	uint64_t sizeAfterEncrypt(); //done
	size_t encryptedIntoBuffer //done
	(vector<bool> const &code, 
		size_t code_start, char* buffer, 
		size_t &ind, const 
		size_t BIT_SIZE);


	vector<char> convertToCharArray(uint64_t number); //done
	size_t roundUp(size_t toRound, size_t sizeOfBlock); //done
private:
	uint64_t alphabet[SIZE_OF_ALPHABET];
	uint64_t numberOfSymbols; //in alphabet
	Node* tree;

	//streams
	std::ifstream fIn;
	std::ofstream fOut;
	
	//results
	vector<char> treeDescription;
	vector<char> orderOfLists;
	vector<bool> codes[SIZE_OF_ALPHABET];
};

#endif //HUFFMAN_ENCRYPT_H
