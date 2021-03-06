/********************************************************************************************//**
 * @file CheckedPtr.h
 *
 * Class tempalte CheckedPtr, from The C++ Programming Language, 2nd - 3rd Editions.
 *
 * ### Synopsys ###
 * template<class T> class CheckedPtr<T>;
 * X a[N];                                   // Array of X's to bind to
 * CheckedPtr<T> p(a, a, a+N);               // Bind to a, position at 1st element
 *
 * ### Formating ###
 * 4 column per hard tab.
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
 * A Run-time, range checked pointer, class template for objects of type T. Once bound to an
 * object, or array of objects, CheckedPtr<T>'s maintains the upper and lower bounds of of the
 * object(s), as well as a current position. Each attempt to dereference a CheckedPtr<T> is
 * range-checked, throwing a range_error exception if the reference is out of bounds.
 *
 * The default constructor results in a unbound pointer that will throw range_errors on every
 * dereference. Unbound pointers can be bound to an object, or arrary of objects, via assignment.
 *
 * @note	Uses default copy constructor and assignment operators.
 *
 * @bug Tests are unfinished
 ************************************************************************************************/
template<class T> class CheckedPtr {
	T*	_pos;									///< Current position in a[]
	T*	_begin;									///< Beginging of array bound to
	T*	_end;									///< End (one pasted last element), of the array

protected:
	void range_check(const T* pos) const {		/// Throw a range_error if pos is out of range
		if (pos == 0 || pos < _begin || pos >= _end) {
			std::ostringstream oss;
			oss  << "CheckedPtr<T>::range_check(" << pos - _begin << ") failed: index is out of bounds";
			throw std::range_error(oss.str());
		}
	}

public:
					CheckedPtr();
					CheckedPtr(T* position);
					CheckedPtr(T* position, T* array, size_t n);
					CheckedPtr(T* position, T* begin, T* end);
	virtual			~CheckedPtr() {}

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

	T&				operator[](size_t i);		///< p[i]
	const T&		operator[](size_t i) const; ///< p[i]

	friend ptrdiff_t operator-(const CheckedPtr<T>& lhs, const CheckedPtr<T>& rhs)
		{	return lhs._pos - rhs._pos;	}
};

/************************************************************************************************
 * publics
 ************************************************************************************************/

/// Default constructor - constructs an unbound CheckedPtr<T>
template<class T>
CheckedPtr<T>::CheckedPtr() : _pos{0},	_begin{0}, _end{0} {}

/// Construct an CheckedPtr<T> bound to an array with a single element
template<class T>
CheckedPtr<T>::CheckedPtr(T* position) : _pos{position}, _begin{position}, _end{position + 1} {}

/// Construct an CheckedPtr<T> bound to an attary of n elements, with initial position @ pos
template<class T>
CheckedPtr<T>::CheckedPtr(T* position, T* array, size_t n) : _pos{position}, _begin{array}, _end{array+n} {}

/// Construct a CheckedPtr<T> bound to array of a span of end - begin
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
T& CheckedPtr<T>::operator[](size_t i) {
	T* p = _begin + i;
	range_check(p);
	return *p;
}

template<class T>
const T& CheckedPtr<T>::operator[](size_t i) const {
	const T* p = _begin + i;
	range_check(p);
	return *p;
}
#endif

