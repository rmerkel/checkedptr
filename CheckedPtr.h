/********************************************************************************************//**
 * @file CheckedPtr.h
 *
 * Class tempalte CheckedPtr, from The C++ Programming Language, 2nd Edition, Section 7.10,
 * 7.14 [19] and 9.9 [2]
 *
 * Formating: 4 column per hard tab.
 *
 * @author Randy Merkel, Slowly but Surly Software.
 * @copyright  (c) 2021 Slowly but Surly Software. All rights reserved.
 ************************************************************************************************/

#ifndef	CHECKEDPTR_H
#define	CHECKEDPTR_H

#include <cassert>
#include <cstddef>
#include <sstream>

/********************************************************************************************//**
 * Range Checked Pointer
 *
 * Runtime range checked pointer for an array of T's. Once bound to an array, CheckedPtr<T>
 * maintains a current position within the array. Any attempt to deference the CheckedPtr<T>
 * whos current position is out of range results in a std::range_error exception.
 *
 * @bug Implementation uses a number of elements in the array, requiring calculating the index of
 * 		the current position with in the array. A more efficient design might use a set of begin,
 *		end pointers.
 * @bug	Lacks a unsigned index operator.
 ************************************************************************************************/
template<class T> class CheckedPtr {
			T*		p;							///< Current position in a[]
	const	T*		a;							///< Array ptr is bound to
			size_t	sz;							///< # of elements in a[]

	void range_check() const					{	range_check(p - a);	}
	void range_check(ptrdiff_t i) const;

public:
	CheckedPtr();
	CheckedPtr(T* pos);
	CheckedPtr(T* pos, const T* array, size_t n);
	CheckedPtr(const CheckedPtr<T>& rhs);

	CheckedPtr<T>&	operator=(const CheckedPtr<T>& rhs);

	CheckedPtr<T>&	operator++();				// ++p
	CheckedPtr<T>	operator++(int);			// p++

	CheckedPtr<T>&	operator+=(int n);			// p += n
	CheckedPtr<T>	operator+(int n) const;		// p + n

	CheckedPtr<T>&	operator--();				// --p
	CheckedPtr<T>	operator--(int);			// p--

	CheckedPtr<T>&	operator-=(int n);			// p -= n
	CheckedPtr<T>	operator-(int n) const;		// p - n

	bool 			operator==(const CheckedPtr<T>& rhs) const;
	bool			operator==(const T* rhs) const;

	T&				operator*();				// *p
	const T&		operator*() const;			// *p

	T*				operator->();				// p->member
	const T&		operator->() const;			// p->member

	T&				operator[](int i);			// p[i]
	const T&		operator[](int i) const;	// p[i]

	friend ptrdiff_t operator-(	const CheckedPtr<T>&	lhs,
								const CheckedPtr<T>&	rhs);
};

/************************************************************************************************
 * privates
 ************************************************************************************************/

/// Throw a range_error if i is out of range of a[0..sz)
template<class T>
void CheckedPtr<T>::range_check(ptrdiff_t i) const {
	if (i < 0 || static_cast<size_t>(i) >= sz) {
		std::ostringstream oss;
		oss << "CheckedPtr<T>::range_check(" << i << ") failed" << std::ends;
		throw std::range_error(oss.str());
	}
}

/************************************************************************************************
 * publics
 ************************************************************************************************/

/// Construct an unbound CheckedPtr<T>
template<class T>
CheckedPtr<T>::CheckedPtr() : p{0},	a{0}, sz{0} {}

/// Construct anCheckedPtr<T> bound to an array with a single element
template<class T>
CheckedPtr<T>::CheckedPtr(T* pos) : p{pos}, a{pos}, sz{1} {}

/// Construct an CheckedPtr<T> bound to an attary of n elements, with initial position pos
template<class T>
CheckedPtr<T>::CheckedPtr(T* pos, const T* array, size_t n) : p{pos}, a{array}, sz{n} {}

/// Construct an CheckedPtr<T> bound to the same array as rhs
template<class T>
CheckedPtr<T>::CheckedPtr(const CheckedPtr<T>& rhs) : p{rhs.p}, a{rhs.a}, sz{rhs.sz} {}

template<class T>
CheckedPtr<T>& CheckedPtr<T>::operator=(const CheckedPtr<T>& rhs) {
	p = rhs.p;
	a = rhs.a;
	sz = rhs.sz;

	return *this;
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator++() {
	++p;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator++(int) {
	p++;
	return CheckedPtr<T>(p - 1, a, sz);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator+=(int n) {
	p += n;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator+(int n) const {
	return CheckedPtr<T>(p + n, a, sz);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator--() {
	--p;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator--(int) {
	p--;
	return CheckedPtr<T>(p + 1, a, sz);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator-=(int n) {
	p -= n;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator-(int n) const {
	return CheckedPtr<T>(p - n, a, sz);
}

template<class T>
bool CheckedPtr<T>::operator==(const CheckedPtr<T>& rhs) const {
	return p == rhs.p;
}

template<class T>
bool CheckedPtr<T>::operator==(const T* rhs) const {
	return p == rhs;
}

template<class T>
T& CheckedPtr<T>::operator*() {
	range_check();
	return *p;
}

template<class T>
const T& CheckedPtr<T>::operator*() const {
	range_check();
	return *p;
}

template<class T>
T* CheckedPtr<T>::operator->() {
	range_check();
	return p;
}

template<class T>
const T& CheckedPtr<T>::operator->() const {
	range_check();
	return *p;
}

template<class T>
T& CheckedPtr<T>::operator[](int i) {
	range_check(&p[i] - a);
	return p[i];
}

template<class T>
const T& CheckedPtr<T>::operator[](int i) const {
	range_check(&p[i] - a);
	return *p;
}

/************************************************************************************************
 * friends
 ************************************************************************************************/

template<class T>
ptrdiff_t operator-(const CheckedPtr<T>& lhs, const CheckedPtr<T>& rhs) {
	return lhs.p - rhs.p;
}

#endif

