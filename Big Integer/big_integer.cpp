#include "big_integer.h"
#include <functional>
#include <cmath>
#include <algorithm>
#include <string>

#include <sstream>

typedef uint32_t u32;
typedef uint64_t u64;

const u32 big_integer::base = 32;
const u32 big_integer::minus = (u32)1 << (base - 1);
const u32 big_integer::minus_block = (u32)(((u64)(1ll << base)) - 1);

bool big_integer::sign() const { //done
	return !data.empty() && (data[data.size() - 1] & minus);
}

bool big_integer::signAfterPop() const { //done
	return data.size() > 1 && (data[data.size() - 2] & minus);
}

u32 big_integer::emptyBlock() const{ //done
	return sign() ? (minus_block) : 0;
}

big_integer big_integer::mulLongShort(big_integer const & a, uint64_t const & b)
{
	u32 b1 = b >> base;
	u32 b2 = b & ((1ll << base) - 1);
	big_integer ans = mulLongShort(a, b1);
	ans <<= base;
	ans += mulLongShort(a, b2);
	return ans;
}

big_integer big_integer::mulLongShort(big_integer const& a, u32 const & b) { //done	
	big_integer ans = a;

	bool sgn = ans.sign();
	if (sgn) {
		ans = -ans;
	}

	ans.data.push_back(0);
	u32 carryFlag = 0;
	for (size_t i = 0; i < ans.data.size(); ++i) {
		u64 cur = (u64)ans.data[i] * b + carryFlag;
		carryFlag = (u32)(cur >> big_integer::base);
		ans.data[i] = (u32)(cur & (((u64)1 << base) - 1));
	}
	if (sgn) {
		ans = -ans;
	}
	ans.cleanEnd();
	return ans;
}

std::pair<big_integer, u32> divLongShort(big_integer const & a, u32 const & b) {
	big_integer ans = a;
	u64 carryFlag = 0, tmp;
	for (size_t i = a.data.size() - 1; i >= 0; --i) {
		tmp = ans.data[i] + (carryFlag << big_integer::base);
		ans.data[i] = (u32)(tmp / b);
		carryFlag = (u32)(tmp % b);
		if (i == 0) break;
	}
	ans.cleanEnd();
	return { ans, carryFlag };
}

void big_integer::cleanEnd() { //done
	while (data.size() > 1 && data[data.size() - 1] == emptyBlock() && sign() == signAfterPop()) { //
		data.pop_back();
	}
}

big_integer::big_integer() { //done
	//data = vector<u32>(1, 0);
	data = Vector();
}
big_integer::big_integer(big_integer const & other) { //done
	data = other.data;
}
big_integer::big_integer(int a) { //done
	data.clear();
	data.push_back((u32)a);
}
big_integer::big_integer(u32 a) { //done
	data.clear();
	data.push_back((u32)a);
	if (sign()) {
		data.push_back(0);
	}
}
big_integer::big_integer(std::string const & str) { //done
	data.clear();
	bool sgn = (str[0] == '-');
	u64 cur = 0;
	u32 pow10 = 1;
	for (char c : str) {
		if (c == '-') continue;
		if (cur * 10 + (c - '0') < ((u32) 1e9) && pow10 < ((u32) 1e9)) {
			cur = cur * 10 + (c - '0');
			pow10 *= 10;
		} else {
			*this = (*this * pow10) + (u32)cur;
			cur = (u64)(c - '0');
			pow10 = 10;
		}
	}
	big_integer tmp = *this * pow10;
	tmp += (u32)cur;
	*this = tmp;
	if (sign()) {
		data.push_back(0);
	}
	if (sgn) {
		*this = -*this; 
	}
}
big_integer::~big_integer() { //done
	data.clear();
}

big_integer & big_integer::operator=(big_integer const & other) { //done
	this->data = std::move(other.data);
	return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
	size_t firstArgSize = data.size();
	u32 empb = emptyBlock();
	data.resize(std::max(data.size(), rhs.data.size()) + 1);
	while (firstArgSize < data.size()) {
		data[firstArgSize++] = empb;
	}
	u32 carryFlag = 0;
	u32 rhsBlock = 0;
	for (size_t i = 0; i < data.size(); ++i) {
		u32 saveCarryFlag = carryFlag;
		rhsBlock = i < rhs.data.size() ? rhs.data[i] : rhs.emptyBlock();
		carryFlag = ((u64)data[i] + rhsBlock + carryFlag) >> big_integer::base;
		data[i] += rhsBlock + saveCarryFlag;
	}
	cleanEnd();
	return *this;
}
big_integer & big_integer::operator-=(big_integer const & rhs) { //done
	return *this += (-rhs);
}

big_integer & big_integer::operator*=(big_integer const & rhs) { //done
	big_integer ans;
	big_integer a = *this, b = rhs;
	bool ansSign = a.sign() ^ b.sign();
	a = a.sign() ? -a : a;
	b = b.sign() ? -b : b;
	for (size_t i = 0; i < b.data.size(); ++i) {
		ans += mulLongShort(a, b.data[i]) << ((u32)i * base);
	}
	ans = ansSign ? -ans : ans;
	ans.cleanEnd();
	return *this = ans;
}


big_integer & big_integer::operator/=(big_integer const & rhs) {
	big_integer res, tmp;
	big_integer a = *this, b = rhs;

	bool ansSign = a.sign() ^ b.sign();
	a = a.sign() ? -a : a;
	b = b.sign() ? -b : b;

	if (a < b) {
		return *this = 0;
	}

	if (a == b) {
		return *this = 1;
	}
	
	if (b.data.size() == 1) {
		res = divLongShort(a, b.data[0]).first;
	}
	else {
		u64 bnorm = b.data[b.data.size() - 1] > 0 ? b.data[b.data.size() - 1] : b.data[b.data.size() - 2];
		u32 norm = (u32)((1ll << base) / (bnorm + 1));
		res.data.resize(a.data.size() - b.data.size() + 1);

		a = mulLongShort(a, norm);
		b = mulLongShort(b, norm);
		
		size_t m = b.data.size() + 1;
		size_t aSize = a.data.size();
		size_t bSize = b.data.size();
		if (b.data[b.data.size() - 1] == 0) {
			--m;
			--bSize;
		}
		if (a.data[a.data.size() - 1] == 0) {
			++m;
			--aSize;
		}
		big_integer firstMFromA;
		firstMFromA.data.resize(m);
		size_t aPointer = a.data.size() - 1;
		for (size_t i = 1; i < m; i++) {
			firstMFromA.data[m - i] = a.data[aPointer--];
		}

		for (size_t k = aSize - bSize - 1; k >= 0; --k) {
			firstMFromA += a.data[aPointer];
			if (aPointer) --aPointer;

			u32 aPrec1, aPrec2;
			if (firstMFromA.data[firstMFromA.data.size() - 1] == 0 && 
				firstMFromA.signAfterPop() != firstMFromA.sign()) {
				aPrec1 = firstMFromA.data[firstMFromA.data.size() - 2];
				aPrec2 = firstMFromA.data[firstMFromA.data.size() - 3];
			}
			else {
				aPrec1 = firstMFromA.data[firstMFromA.data.size() - 1];
				aPrec2 = firstMFromA.data[firstMFromA.data.size() - 2];
			}
			u32 bPrec = b.data[b.data.size() - 1] == 0 ? b.data[b.data.size() - 2] : b.data[b.data.size() - 1];
			u64 y = precalc(aPrec1, aPrec2, bPrec);
			u32 carry = y >> base;
			
			if (carry) {
				if (k == res.data.size() - 1) {
					res.data.push_back(carry);
				}
				else {
					res.data[k + 1] += carry;
				}
			}

			if (firstMFromA.sign()) {
				firstMFromA.data.push_back(0);
			}
			big_integer ppp = mulLongShort(b, y);

			while (ppp > firstMFromA) {
				--y;
				ppp = mulLongShort(b, y);
			}

			firstMFromA -= ppp;
			res.data[k] = y & ((1ll << base) - 1);
			firstMFromA <<= base;
			
			if (k == 0) break;
		}
		
		if (res.sign()) {
			res.data.push_back(0);
		}
	}
	res = ansSign ? -res : res;
	res.cleanEnd();
	return *this = res;
}

uint64_t big_integer::precalc(uint32_t a, uint32_t b, uint32_t c) {
	u64 ab = ((u64)a << big_integer::base) + b;
	u64 res = ab / c;
	return res;
}
big_integer & big_integer::operator%=(big_integer const & rhs) { //done
	return *this -= (*this / rhs) * rhs;
}

big_integer &big_integer::apply(big_integer const &rhs, const std::function<u32(u32, u32)> operation) {
	size_t firstArgSize = data.size();
	data.resize(std::max(data.size(), rhs.data.size()));

	while (firstArgSize < data.size()) {
		data[firstArgSize++] = emptyBlock();
	}
	for (size_t i = 0; i < data.size(); ++i) {
		data[i] = operation(data[i], i < rhs.data.size() ? rhs.data[i] : rhs.emptyBlock());
	}
	cleanEnd();
	return *this;
}

big_integer & big_integer::operator&=(big_integer const & rhs) { //done
	return apply(rhs, 
		[](u32 a, u32 b) { 
			return a & b; 
		});
}
big_integer & big_integer::operator|=(big_integer const & rhs) { //done
	return apply(rhs, 
		[](u32 a, u32 b) { 
			return a | b; 
		});
}
big_integer & big_integer::operator^=(big_integer const & rhs) { //done
	return apply(rhs, 
		[](u32 a, u32 b) { 
			return a ^ b; 
		});
}

void big_integer::shiftBlocks(int rhs) { //done
	if (rhs > 0) {
		data.resize(data.size() + rhs);
		for (size_t i = data.size() - rhs; i > 0; --i) {
			data[i + rhs - 1] = data[i - 1];
		}
		for (size_t i = (size_t)rhs; i > 0; --i) {
			data[i - 1] = 0;
		}
	}
	else {
		for (auto i = (size_t)-rhs; i < data.size(); ++i) {
			data[i + rhs] = data[i];
		}
		u32 empb = emptyBlock();
		for (size_t i = data.size(); i > data.size() + rhs; --i) {
			data[i - 1] = empb;
		}
		cleanEnd();
	}
}

big_integer & big_integer::operator<<=(int rhs) { //done
	if (rhs < 0) {
		return *this >>= -rhs;
	}
	int needToBeShifted = rhs / base;
	if (needToBeShifted) {
		shiftBlocks(needToBeShifted);
	}
	needToBeShifted = rhs - needToBeShifted * base;
	if (needToBeShifted) { 
		data.push_back(emptyBlock());
		for (size_t i = data.size(); i > 0; --i) {
			if (i != data.size()) {
				data[i] += data[i - 1] >> (base - needToBeShifted);
			}
			data[i - 1] <<= needToBeShifted;
		}
	}
	cleanEnd();
	return *this;
}
big_integer & big_integer::operator>>=(int rhs) { //done
	if (rhs < 0) {
		return *this <<= -rhs;
	}
	int needToBeShifted = rhs / base;
	if (needToBeShifted) {
		shiftBlocks(-needToBeShifted);
	}
	needToBeShifted = rhs - needToBeShifted * base;
	if (needToBeShifted) {
		u32 empb = emptyBlock();
		for (size_t i = 0; i < data.size(); ++i) {
			if (i != 0) {
				data[i - 1] += data[i] << (base - needToBeShifted);
			}
			data[i] >>= needToBeShifted;
		}
		data[data.size() - 1] += empb << (base - needToBeShifted);
	}
	cleanEnd();
	return *this;
}

big_integer big_integer::operator+() const { //done
	return *this;
}
big_integer big_integer::operator-() const { //done
	big_integer comp = ~*this + 1;
	return comp;
}
big_integer big_integer::operator~() const { //done
	big_integer cur = *this;
	if (cur.data.empty()) {
		cur.data.push_back(0);
	}
	for (size_t i = 0; i < cur.data.size(); ++i) {
		cur.data[i] = ~cur.data[i];
	}
	cur.cleanEnd();
	return cur;
}

big_integer & big_integer::operator++() { //done
	return *this += 1;
}
big_integer & big_integer::operator--() { //done
	return *this -= 1;
}

bool operator==(big_integer const & a, big_integer const & b) { //data
	return a.data == b.data;
}
bool operator!=(big_integer const & a, big_integer const & b) { //data
	return !(a.data == b.data);
}
bool operator<(big_integer const & a, big_integer const & b) { //done
	if (a.sign() != b.sign()) {
		return a.sign();
	}
	if (a.data.size() != b.data.size()) {
		return (a.data.size() < b.data.size()) ^ a.sign();
	}
	for (size_t i = a.data.size(); i > 0; --i) {
		if (a.data[i - 1] != b.data[i - 1]) {
			return a.data[i - 1] < b.data[i - 1];
		}
	}
	return false;
}
bool operator>(big_integer const & a, big_integer const & b) { //done
	return (!(a < b)) && (a != b);
}
bool operator<=(big_integer const & a, big_integer const & b) { //done
	return (a < b) || (a == b);
}
bool operator>=(big_integer const & a, big_integer const & b) { //done
	return (a > b) || (a == b);
}

std::string to_string(big_integer const & a) { //done
	std::string res;
	big_integer s = a;
	if (s.sign()) {
		s = -s;
		res += '-';
	}
	std::vector<std::string> blocksInString(s.data.size() * 2);
	size_t vSize = 0;
	while (s > 0) {
		std::pair<big_integer, u32> takeBlock = divLongShort(s, (u32)1e9);
		blocksInString[vSize++] = std::to_string(takeBlock.second);
		s = takeBlock.first;
	}
	for (size_t i = vSize; i > 0; --i) {
		if (i != vSize) {
			res += std::string(9 - blocksInString[i - 1].size(), '0');
		}
		res += blocksInString[i - 1];
	}
	if (res.empty()) {
		res = "0";
	}
	return res;
}

std::ostream & operator<<(std::ostream & s, big_integer const & a) { //done
	return s << to_string(a);
}

big_integer operator+(big_integer a, big_integer const & b) { //done
	return a += b;
}
big_integer operator-(big_integer a, big_integer const & b) { //done
	return a -= b;
}
big_integer operator*(big_integer a, big_integer const & b) { //done
	return a *= b;
}
big_integer operator/(big_integer a, big_integer const & b) { //done
	return a /= b;
}
big_integer operator%(big_integer a, big_integer const & b) { //done
	return a %= b;
}

big_integer operator&(big_integer a, big_integer const & b) { //done
	return a &= b;
}
big_integer operator|(big_integer a, big_integer const & b) { //done
	return a |= b;
}
big_integer operator^(big_integer a, big_integer const & b) { //done
	return a ^= b;
}

big_integer operator<<(big_integer a, int b) { //done
	return a <<= b;
}
big_integer operator>>(big_integer a, int b) { //done
	return a >>= b;
}
