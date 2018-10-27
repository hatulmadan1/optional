
#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <cassert>

template<class T>
struct optional {
private:
	char data[sizeof(T)];
	bool containsSmth;
public:
	//constructors:
	optional(); //done
	optional(T const &other); //done
	optional(optional const &other); //done
	~optional(); //done

	optional &operator=(optional const &other); //done

	void clear(); //done

	explicit operator bool() const; //done

	T &operator*();

	T const &operator*() const;

	T *operator->();

	T const *operator->() const;

	friend bool operator==(optional const &a, optional const &b) {
		if (!a || !b) {
			return !a == !b;
		}
		return *a == *b;
	}

	friend bool operator!=(optional<T> const &a, optional<T> const &b) {
		return !(a == b);
	}

	friend bool operator<(optional<T> const &a, optional<T> const &b) {
		if (a.containsSmth && b.containsSmth) {
			return *a < *b;
		}
		if (!a.containsSmth && !b.containsSmth) {
			return false;
		}
		return !a.containsSmth;
	}

	friend bool operator>(optional<T> const &a, optional<T> const &b) {
		return b < a;
	}

	friend bool operator<=(optional<T> const &a, optional<T> const &b) {
		return (a < b) || a == b;
	}

	friend bool operator>=(optional<T> const &a, optional<T> const &b) {
		return (a > b) || a == b;
	}

	void swap(optional &other) {
		optional temp = other;
		other = *this;
		*this = temp;
	}

};

template<typename T>
inline optional<T>::optional() { //done
	containsSmth = false;
}

template<typename T>
inline optional<T>::optional(T const &other) {  //done
	new(data) T(other);
	containsSmth = true;
}

template<typename T>
inline optional<T>::optional(optional const &other) : containsSmth(other.containsSmth) { //done
	if (containsSmth) {
		new (data) T(*other);
	}
	containsSmth = other.containsSmth;
}

template<typename T>
inline optional<T>::~optional() { //done
	clear();
}

template<typename T>
inline optional<T> &optional<T>::operator=(optional<T> const &other) { //done
	if (containsSmth && other.containsSmth) {
		(T &)(data) = *other;
	}
	else if (containsSmth && !other.containsSmth) {
		clear();
	}
	else if (!containsSmth && other.containsSmth) {
		new (data) T(*other);
		containsSmth = true;
	}
	return *this;
}

template<class T>
inline void optional<T>::clear() { //done
	if (containsSmth) {
		operator*().~T();
	}
	containsSmth = false;
}

template<class T>
inline optional<T>::operator bool() const {//done
	return containsSmth;
}

template<class T>
inline T &optional<T>::operator*() {
	assert(containsSmth);
	return reinterpret_cast<T &>(data);
}

template<class T>
inline T const &optional<T>::operator*() const {
	assert(containsSmth);
	return reinterpret_cast<T const &>(data);
}

template<class T>
inline T *optional<T>::operator->() {
	return &operator*();
}

template<class T>
inline T const *optional<T>::operator->() const {
	return &operator*();
}

template<typename T>
void swap(optional<T> &a, optional<T> &b) {
	a.swap(b);
}

#endif // !OPTIONAL_H
