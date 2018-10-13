#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <memory>

struct Vector {
private:
	union {
		std::shared_ptr<std::vector<uint32_t>> big;
		uint32_t small;
	};

	size_t sizeOfObject;
	bool isSmallObject() const;

	void unique_checker();
public:
	Vector(); //constructor
	~Vector(); //destructor

	//informtion about size
	size_t size() const; 
	bool empty() const;

	//main functions
	void push_back(uint32_t val);
	void pop_back();
	void resize(size_t num_of_elements);
	void clear();

	//operators
	Vector &operator=(Vector const &x);
	friend bool operator==(Vector const &first, Vector const &second);
	uint32_t &operator[](size_t id);
	uint32_t operator[](size_t id) const;
};

bool operator==(Vector const &first, Vector const &second);

#endif // !VECTOR_H

