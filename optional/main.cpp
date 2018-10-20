#include <iostream>
#include "optional.h"

struct A {
	int x;
	A() = delete;
	A(int v) : x(v) {}

};

int main() {
	optional<A> opt{ A(123) };
	std::cout << opt->x << std::endl;

	optional<A> b{ 8 };
	std::cout << (*b).x << std::endl;

	swap(opt, b);
	assert(opt->x == 8 && b->x == 123);
}