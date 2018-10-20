#pragma warning(disable : 4996)
#include<algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include "gtest/gtest.h"
#include "HufLib/decrypt.h"
#include "HufLib/encrypt.h"
#include <sstream>

namespace {
	std::string input(std::string str) {
		std::ofstream in_test("test.txt", std::ios::binary);
		in_test << str;
		in_test.close();
		encrypter e;

		e.encrypt("test.txt", "output.txt");
		decrypter d;
		d.decrypt("output.txt", "decrypt.txt");
		std::ifstream out_test("decrypt.txt", std::ios::binary);
		std::string check((std::istreambuf_iterator<char>(out_test)),
			std::istreambuf_iterator<char>());
		out_test.close();
		return check;
	}

	bool generate(size_t size) {
		std::ofstream in_test("test.txt", std::ios::binary);
		char* in = new char[size];
		for (size_t i = 0; i < size; ++i) {
			in[i] = (char)rand();
		}
		in_test.write(in, size);
		in_test.close();
		encrypter e;
		e.encrypt("test.txt", "output.txt");
		decrypter d;
		d.decrypt("output.txt", "decrypt.txt");
		std::ifstream out_test("decrypt.txt", std::ios::binary);
		char* out = new char[size];
		out_test.read(out, size);
		bool result = true;
		for (size_t i = 0; i < size; ++i) {
			if (in[i] != out[i]) {
				result = false;
				break;
			}
		}
		delete[] in;
		delete[] out;
		return result;
	}

	bool equal(std::string file1, std::string file2, std::string file3)
	{
		encrypter e;
		e.encrypt(file1, file2);
		decrypter d;
		d.decrypt(file2, file3);

		char *buf1 = new char[SIZE_OF_BUFFER];
		char *buf2 = new char[SIZE_OF_BUFFER];

		std::ifstream f1(file1);
		std::ifstream f3(file3);

		while (!f1.eof() && !f3.eof()) {
			if (f1.eof() ^ f3.eof())
				return false;
			f1.read(buf1, SIZE_OF_BUFFER);
			f3.read(buf2, SIZE_OF_BUFFER);
			for (int i = 0; i < SIZE_OF_BUFFER; ++i)
				if (buf1[i] != buf2[i])
					return false;
		}
		return true;
	}
}

TEST(correctness, hello_world)
{
	std::string str = "hello world";
	std::string check = input(str);
	EXPECT_EQ(str, check);
}

TEST(correctness, empty)
{
	std::string str = "";
	std::string check = input(str);
	EXPECT_EQ(str, check);
}

TEST(correctness, one_letter)
{
	std::string str = "a";
	std::string check = input(str);
	EXPECT_EQ(str, check);

	str = "aaaaaaa";
	check = input(str);
	EXPECT_EQ(str, check);
}

namespace {

}

TEST(correctness, big_random)
{
	for (size_t i = 0; i < 10; ++i) {
		EXPECT_TRUE(generate(1000));
	}
}

TEST(correctness, buffer) {
	EXPECT_TRUE(generate(SIZE_OF_BUFFER * 4));
}

TEST(correctness, png) {
	EXPECT_TRUE(equal("Tests/first_test1.jpg", "Tests/first_test2.jpg", "Tests/first_test3.jpg"));
}

TEST(correctness, png2) {
	EXPECT_TRUE(equal("Tests/second_test1.jpg", "Tests/second_test2.jpg", "Tests/second_test3.jpg"));
}

TEST(correctness, jpg_big) {
	EXPECT_TRUE(equal("Tests/fifth_test1.jpg", "Tests/fifth_test2.jpg", "Tests/fifth_test3.jpg"));
}
