#include "decrypt.h"

#include <algorithm>

void decrypter::decrypt(std::string fileIn, std::string fileOut) {
	fIn.open(fileIn, std::ios::binary);
	if (!fIn.is_open()) { throw FilestreamCorruptedException(); }

	fOut.open(fileOut, std::ios::binary);
	if (!fOut.is_open()) {
		fIn.close();
		throw FilestreamCorruptedException();
	}

	size_t sizeofNumberOfSymbols = roundUp(sizeof(numberOfSymbols), sizeof(char));
	size_t sizeofSizeAfterEncrypt = roundUp(sizeof(sizeAfterEncrypt), sizeof(char));

	char *charNumberOfSymbols = new char[sizeofNumberOfSymbols];
	char *charSizeAfterEncrypt = new char[sizeofSizeAfterEncrypt];

	fIn.read(charNumberOfSymbols, sizeof(numberOfSymbols));

	if (fIn.gcount() < sizeofNumberOfSymbols) {
		delete[] charSizeAfterEncrypt, delete[] charNumberOfSymbols;
		cleaner();
	}

	numberOfSymbols = convertToNumber(charNumberOfSymbols, sizeofNumberOfSymbols);
	if (!numberOfSymbols) {
		fIn.close(), fOut.close();
		delete[] charNumberOfSymbols, delete[] charSizeAfterEncrypt;
		return;
	}

	fIn.read(charSizeAfterEncrypt, sizeof(sizeAfterEncrypt));
	if (fIn.gcount() < sizeofSizeAfterEncrypt) {
		delete[] charNumberOfSymbols, delete[] charSizeAfterEncrypt;
		cleaner();
	}

	sizeAfterEncrypt = convertToNumber(charSizeAfterEncrypt, sizeofSizeAfterEncrypt);
	delete[] charNumberOfSymbols, delete[] charSizeAfterEncrypt;

	if (numberOfSymbols == 1) ++numberOfSymbols;

	size_t sizeofTreeDescription = roundUp(2 * (numberOfSymbols - 1), 8);
	char* treeDescription = new char[sizeofTreeDescription];

	fIn.read(treeDescription, sizeofTreeDescription);
	if (fIn.gcount() < sizeofTreeDescription) { cleaner(); }

	tree = new Node;
	build(treeDescription, 2 * (numberOfSymbols - 1));
	dfs(tree);

	char* buffer_in = new char[SIZE_OF_BUFFER];
	char* buffer_out = new char[SIZE_OF_BUFFER];

	size_t indBuf = 0;
	Node *versKeeper = nullptr;

	while (sizeAfterEncrypt > 0) {
		if (fIn.eof()) {
			delete[] buffer_in, delete[] buffer_out, delete tree;
			cleaner();
		}

		fIn.read(buffer_in, SIZE_OF_BUFFER);
		auto sizeInByte = fIn.gcount();

		size_t end = std::min(sizeInByte * 8, sizeAfterEncrypt);
		sizeAfterEncrypt -= sizeInByte * 8;

		for (size_t i = 0; i < end;) {
			Node *vers;
			if (versKeeper != nullptr) {
				vers = versKeeper;
				versKeeper = nullptr;
			}
			else { vers = tree; }

			while (vers->left != nullptr) {
				if (i == end) {
					versKeeper = vers;
					break;
				}
				vers = (bitValue(buffer_in, i++)) ? vers->left : vers->right;
			}

			if (versKeeper != nullptr) { break; }

			buffer_out[indBuf++] = vers->value;
			if (indBuf == SIZE_OF_BUFFER) {
				fOut.write(buffer_out, SIZE_OF_BUFFER);
				indBuf = 0;
			}
		}
	}

	if (indBuf > 0) {
		fOut.write(buffer_out, indBuf);
	}

	delete[] buffer_in, delete[] buffer_out, delete tree;
	fIn.close();
	fOut.close();

}

void decrypter::build(char * treeDescription, size_t sizeAfterEncrypt) { //done
	Node *cur = tree;
	for (size_t i = 0; i < sizeAfterEncrypt; ++i) {
		if (bitValue(treeDescription, i)) {
			Node *left = new Node(cur);
			cur->left = left;
			cur = left;
		}
		else {
			while (true) {
				cur = cur->v;
				if (cur->v == nullptr || cur->right == nullptr) break;
			}
			Node *right = new Node(cur);
			cur->right = right;
			cur = right;
		}
	}
}

void decrypter::dfs(Node * vers) { //done
	if ((vers->left == nullptr) ^ (vers->right == nullptr)) { cleaner(); }
	if (vers->left == nullptr) {
		char ch;
		fIn.read(&ch, 1);
		vers->value = (unsigned char)(ch);
	}
	else {
		dfs(vers->left);
		dfs(vers->right);
	}
}

void decrypter::cleaner() { //done
	fIn.close();
	fOut.close();
	throw FileCorruptedException();
}

bool decrypter::bitValue(char * array, size_t ind) { //done
	return ((array[ind / (sizeof(char) * 8)] >> (sizeof(char) * 8 - 1 - ind % (sizeof(char) * 8))) & 1);
}

uint64_t decrypter::convertToNumber(char * data, size_t size) { //done
	uint64_t number = 0;
	size_t step = (sizeof(uint64_t) - sizeof(char)) * 8;
	for (size_t i = 0; i < size; ++i) {
		number |= (uint64_t)(unsigned char)data[i] << step;
		step -= sizeof(char) * 8;
	}
	return number;
}

size_t decrypter::roundUp(size_t toRound, size_t sizeOfBlock) { //done
	return (toRound + (sizeOfBlock - 1)) / sizeOfBlock;
}