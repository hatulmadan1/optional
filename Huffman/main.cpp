#include "HufLib/Node.h"
#include "HufLib/encrypt.h"
#include "HufLib/decrypt.h"
#include <iterator>
#include <sstream>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
	std::ios_base::sync_with_stdio(0);
	cin.tie(0);
	while (true) {
		bool encdec = true; //true - encrypt false - decrypt
		while (true) {
			cout << "Encrypt or Decrypt file? [e/d]" << endl;
			char c;
			cin >> c;
			if (c == 'e' || c == 'E') {
				encdec = true;
				break;
			}
			else if (c == 'd' || c == 'D') {
				encdec = false;
				break;
			}
		}
		cout << "Enter name of input and output files" << endl;
		std::string infile, outfile;
		cin >> infile >> outfile;
		if (encdec) {
			try {
				encrypter e;
				e.encrypt(infile, outfile);
				cout << "Encoding was completed successful" << endl;
			}
			catch (FilestreamCorruptedException) {
				cout << "Couldn't open file" << endl;
			}
		}
		else {
			try {
				decrypter d;
				d.decrypt(infile, outfile);
				cout << "Decrypted successful" << endl;
			}
			catch (FilestreamCorruptedException) {
				cout << "Couldn't open";
			}
			catch (FileCorruptedException) {
				cout << "File can't be decrypted because of corruption" << endl;
			}
		}
		cout << "Do you want to do anything else? [y/n]" << endl;
		char exit;
		cin >> exit;
		if (exit == 'n' || exit == 'N') {
			cout << "Good bye!" << endl;
			break;
		}
	}
}
