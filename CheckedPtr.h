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

#include <cstddef>
#include <sstream>

/********************************************************************************************//**
 * Range Checked Pointer
 *
 * Run-time range checked pointer for a T. Once bound to an array, a CheckedPtr<T> maintians a
 * position with in the array, and the upper and lower bounds of the array. Each attempt to
 * dereference a CheckedPtr<T> results in a range-check of the current position, throwing a
 * range_error exception if the check fails.
 *
 * @note	Attempts to derefernce a unbound CheckedPtr<T> will result in a throw range_error.
 *
 * @note	Uses default copy constructor and assignment operators.
 *
 * @bug	Lacks a unsigned index operator.
 * @bug Tests are unfinished
 ************************************************************************************************/
template<class T> class CheckedPtr {
	T*	_pos;									///< Current position in a[]
	T*	_begin;									///< Beginging of array bound to
	T*	_end;									///< End (one pasted last element), of the array

	void range_check(const T* pos) const;

public:
	CheckedPtr();
	CheckedPtr(T* position);
	CheckedPtr(T* position, T* array, size_t n);
	CheckedPtr(T* position, T* begin, T* end);

	CheckedPtr<T>&	operator++();				///< ++p
	CheckedPtr<T>	operator++(int);			///< p++

	CheckedPtr<T>&	operator+=(int n);			///< p += n
	CheckedPtr<T>	operator+(int n) const;		///< p + n

	CheckedPtr<T>&	operator--();				///< --p
	CheckedPtr<T>	operator--(int);			///< p--

	CheckedPtr<T>&	operator-=(int n);			///< p -= n
	CheckedPtr<T>	operator-(int n) const;		///< p - n

	bool 			operator==(const CheckedPtr<T>& rhs) const;
	bool			operator==(const T* rhs) const;

	bool 			operator!=(const CheckedPtr<T>& rhs) const;
	bool			operator!=(const T* rhs) const;

	T&				operator*();				///< *p
	const T&		operator*() const;			///< *p

	T*				operator->();				///< p->member
	const T&		operator->() const;			///< p->member

	T&				operator[](int i);			///< p[i]
	const T&		operator[](int i) const;	///< p[i]

	friend ptrdiff_t operator-(	const CheckedPtr<T>& lhs,
								const CheckedPtr<T>& rhs) {
		return lhs._pos - rhs._pos;
	}
		
};

/************************************************************************************************
 * privates
 ************************************************************************************************/

/// Throw a range_error if pos is out of range
template<class T>
void CheckedPtr<T>::range_check(const T* pos) const {
	std::ostringstream oss;

	if (pos == 0 || _begin == 0 || _end == 0) {
		oss << "CheckedPtr<T>::range_check(" << pos - _begin << ") failed: unbound";
		throw std::range_error(oss.str());

	} else if (pos < _begin || pos >= _end) {
		oss  << "CheckedPtr<T>::range_check(" << pos - _begin << ") failed: out of bounds";
		throw std::range_error(oss.str());
	}
}

/************************************************************************************************
 * publics
 ************************************************************************************************/

/// Construct an unbound CheckedPtr<T>
template<class T>
CheckedPtr<T>::CheckedPtr() : _pos{0},	_begin{0}, _end{0} {}

/// Construct anCheckedPtr<T> bound to an array with a single element
template<class T>
CheckedPtr<T>::CheckedPtr(T* position) : _pos{position}, _begin{position}, _end{position + 1} {}

/// Construct an CheckedPtr<T> bound to an attary of n elements, with initial position pos
template<class T>
CheckedPtr<T>::CheckedPtr(T* position, T* array, size_t n) : _pos{position}, _begin{array}, _end{array+n} {}

template<class T>
CheckedPtr<T>::CheckedPtr(T* position, T* begin, T* end) : _pos{position}, _begin{begin}, _end{end} {}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator++() {
	++_pos;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator++(int) {
	_pos++;
	return CheckedPtr<T>(_pos);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator+=(int n) {
	_pos += n;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator+(int n) const {
	return CheckedPtr<T>(_pos + n, _pos, _end);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator--() {
	--_pos;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator--(int) {
	_pos--;
	return CheckedPtr<T>(_pos + 1, _begin, _end);
}

template<class T>
CheckedPtr<T>&	CheckedPtr<T>::operator-=(int n) {
	_pos -= n;
	return *this;
}

template<class T>
CheckedPtr<T>	CheckedPtr<T>::operator-(int n) const {
	return CheckedPtr<T>(_pos - n, _begin, _end);
}

template<class T>
bool CheckedPtr<T>::operator==(const CheckedPtr<T>& rhs) const {
	return _pos == rhs._pos;
}

template<class T>
bool CheckedPtr<T>::operator==(const T* rhs) const {
	return _pos == rhs;
}

template<class T>
bool CheckedPtr<T>::operator!=(const CheckedPtr<T>& rhs) const {
	return _pos != rhs._pos;
}

template<class T>
bool CheckedPtr<T>::operator!=(const T* rhs) const {
	return _pos != rhs;
}

template<class T>
T& CheckedPtr<T>::operator*() {
	range_check(_pos);
	return *_pos;
}

template<class T>
const T& CheckedPtr<T>::operator*() const {
	range_check(_pos);
	return *_pos;
}

template<class T>
T* CheckedPtr<T>::operator->() {
	range_check(_pos);
	return _pos;
}

template<class T>
const T& CheckedPtr<T>::operator->() const {
	range_check(_pos);
	return *_pos;
}

template<class T>
T& CheckedPtr<T>::operator[](int i) {
	range_check(&_begin[i]);
	return _begin[i];
}

template<class T>
const T& CheckedPtr<T>::operator[](int i) const {
	range_check(&_begin[i]);
	return _begin[i];
}

#endif

