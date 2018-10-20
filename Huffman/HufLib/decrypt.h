#ifndef HUFFMAN_DECRYPT_H
#define HUFFMAN_DECRYPT_H

#include "Node.h"

class decrypter {
public:
	void decrypt(std::string fileIn, std::string fileOut);
private:
	void build(char *treeDescription, size_t sizeAfterEncrypt); //done
	void dfs(Node *vers);  //done
	void cleaner(); //done

	bool bitValue(char *array, size_t ind); //done
	uint64_t convertToNumber(char* data, size_t size); //done

	size_t roundUp(size_t toRound, size_t sizeOfBlock); //done
private:
	int64_t sizeAfterEncrypt;
	uint64_t numberOfSymbols;
	Node* tree;
	std::ifstream fIn;
	std::ofstream fOut;
};

#endif //HUFFMAN_DECRYPT_H
