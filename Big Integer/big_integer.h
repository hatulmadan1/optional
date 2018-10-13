#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "Vector.h"
#include <functional>

struct big_integer {
private:
	Vector data;

	static const uint32_t base;
	static const uint32_t minus;
	static const uint32_t minus_block;

	bool sign() const; //done
	bool signAfterPop() const; //done
public:
	big_integer(); //done
	big_integer(big_integer const& other); //done
	big_integer(int a); //done
	big_integer(uint32_t a); //done
	explicit big_integer(std::string const& str); //done
	~big_integer(); //done

	big_integer& operator=(big_integer const& other); //done

	big_integer& operator+=(big_integer const& rhs); //done
	big_integer& operator-=(big_integer const& rhs); //done
	big_integer& operator*=(big_integer const& rhs); //done
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs); //done

	big_integer& apply(big_integer const & rhs, 
		std::function<uint32_t(uint32_t, uint32_t)> operation); //done
	big_integer& operator&=(big_integer const& rhs); //done
	big_integer& operator|=(big_integer const& rhs); //done
	big_integer& operator^=(big_integer const& rhs); //done

	void shiftBlocks(int rhs); //done
	big_integer& operator<<=(int rhs); //done
	big_integer& operator>>=(int rhs); //done

	big_integer operator+() const; //done
	big_integer operator-() const; //done
	big_integer operator~() const; //done

	big_integer& operator++(); //done
	big_integer& operator--(); //done

	friend bool operator==(big_integer const& a, big_integer const& b); //done
	friend bool operator!=(big_integer const& a, big_integer const& b); //done
	friend bool operator<(big_integer const& a, big_integer const& b); //done
	friend bool operator>(big_integer const& a, big_integer const& b); //done
	friend bool operator<=(big_integer const& a, big_integer const& b); //done
	friend bool operator>=(big_integer const& a, big_integer const& b); //done

	friend std::string to_string(big_integer const& a); //done

	void cleanEnd(); //done
	uint32_t emptyBlock() const; //done
	big_integer mulLongShort(big_integer const& a, uint32_t const& b); //done
	friend std::pair<big_integer, uint32_t> divLongShort(big_integer const &a, uint32_t const &b);
};

big_integer operator+(big_integer a, big_integer const& b); //done
big_integer operator-(big_integer a, big_integer const& b); //done
big_integer operator*(big_integer a, big_integer const& b); //done
big_integer operator/(big_integer a, big_integer const& b); //done
big_integer operator%(big_integer a, big_integer const& b); //done

big_integer operator&(big_integer a, big_integer const& b); //done
big_integer operator|(big_integer a, big_integer const& b); //done
big_integer operator^(big_integer a, big_integer const& b); //done

big_integer operator<<(big_integer a, int b); //done
big_integer operator>>(big_integer a, int b); //done

bool operator==(big_integer const& a, big_integer const& b); //done
bool operator!=(big_integer const& a, big_integer const& b); //done
bool operator<(big_integer const& a, big_integer const& b); //done
bool operator>(big_integer const& a, big_integer const& b); //done
bool operator<=(big_integer const& a, big_integer const& b); //done
bool operator>=(big_integer const& a, big_integer const& b); //done

std::string to_string(big_integer const& a); //done
std::ostream& operator<<(std::ostream& s, big_integer const& a); //done

std::pair<big_integer, uint32_t> divLongShort(big_integer const &a, uint32_t const &b);

#endif // BIG_INTEGER_H