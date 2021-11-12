/********************************************************************************************//**
 * @file xCheckedPtr.cc
 *
 * CheckedPtr<T> tests.
 *
 * ### Formating ###
 * 4 column per hard tab.
 *
 * @author Randy Merkel, Slowly but Surly Software.
 * @copyright  (c) 2021 Slowly but Surly Software. All rights reserved.
 ************************************************************************************************/

#include "CheckedPtr.h"

#include <iostream>

using namespace std;

/********************************************************************************************//**
 * Simple class to test CheckedPtr<X> with.
 ************************************************************************************************/
class X {
	static unsigned n;
	unsigned i;

public:
	X() : i{n++} {}
	void print() const;
};

unsigned X::n = 0;

void X::print() const {	cout << "X::n: == " << n << "X::i: == " << i << '\n';	}

/********************************************************************************************//**
 * Explicit instantiations
 ************************************************************************************************/
template class CheckedPtr<int>;
template class CheckedPtr<X>;

/********************************************************************************************//**
 * CheckedPtr<T> test interface
 ************************************************************************************************/
template<class P, class T>
class Test {
protected:
	static const size_t size = 10;
	T a[size];

public:
	virtual bool operator()() = 0;
};

/********************************************************************************************//**
 * run one test on one type
 ************************************************************************************************/
template<class P, class T>
static bool do_one_test(const char* type, int testn, Test<P, T>& func) {
	cout << "CheckedPtr<T>: <" << type << "> test #" << testn << "..." << '\n';
	bool okay = func();
	cout << '<' << type << "> Test #" << testn << (okay? " Passed " : " Failed!") << '\n';
	cout << '\n';

	return okay;
}

/********************************************************************************************//**
 * Test #1 - test CheckedPtr<T>::operator --,+= and *
 ************************************************************************************************/
template<class P, class T>
class test1 : public Test<P, T> {
public:
	bool operator()();
};

template<class P, class T>
bool test1<P, T>::operator()() {
	T x{};
	P p(this->a, this->a, this->size);
	unsigned expected = 0;

	try {
		--p;								// underrun test
		try {
			*p = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}

		++p;
		*p = x;
											// underrun test
		for (int i = 0; i < static_cast<int>(this->size); ++i)
			++p;

		try {
			*p = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}
		--p;
		*p;

		for (P p(this->a, this->a, this->size); p != &this->a[this->size]; ++p)
			;								// pre-increment test

		for (P p(this->a, this->a, this->size); p != &this->a[this->size]; p++)
			;								// post-increment test

	} catch (range_error& r) {
		cerr << "Unexpected rnage_error: " << r.what() << endl;
		return false;
	}

	return expected == 2;
}

static bool do_test1() {
	test1<CheckedPtr<int>, int>	int_test;
	test1<CheckedPtr<X>, X>		X_test;

	return do_one_test("int", 1, int_test) && do_one_test("X", 1, X_test);
}

/********************************************************************************************//**
 * Test #2 - test CheckedPtr<T>::operator[] and *
 ************************************************************************************************/
template<class P, class T>
class test2 : public Test<P, T> {
public:
	bool operator()();
};

template<class P, class T>
bool test2<P, T>::operator()() {
	T x{};
	P p(this->a, this->a, this->size);
	unsigned expected = 0;

	try {
		// non-const tests
		P p(this->a, this->a, this->size);
		int i = 0;							// index into p
		p[i--];								// underrun test
		try { 
			p[i] = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}
		p[++i] = x;

		i = this->size;					// overrun test
		try {
			p[i] = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}
		p[i = this->size-1] = x;

		// const tests

		const P p1(this->a - 1, this->a, this->size);
		try {								// underrun test
			x = *p1;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}

		const P p2(this->a, this->a, this->size);
		x = *p2;

		const P p3(this->a + this->size, this->a, this->size);
		try {								// underrun test
			x = *p3;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}

	} catch (range_error& r) {
		cerr << "Unexpected range_error: " << r.what() << "!\n";
		return false;
	}

	return expected == 4;
}

static bool do_test2() {
	test2<CheckedPtr<int>, int>	int_test;
	test2<CheckedPtr<X>, X>		X_test;

	return do_one_test("int", 2, int_test) && do_one_test("X", 2, X_test);
}

/********************************************************************************************//**
 * Test #3 - test CheckedPtr<T>::operator=+, -+ and *
 ************************************************************************************************/
template<class P, class T>
class test3 : public Test<P, T> {
public:
	bool operator()();
};

template<class P, class T>
bool test3<P, T>::operator()() {
	unsigned expected = 0;
	T x{};

	try {
		P p(this->a, this->a, this->size);
		p -= 1;								// underrun test
		try {
			*p = x;

		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}
		p += 1;
		*p = x;

		p+= this->size;						// overrun test
		try {
			*p = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
			++expected;
		}
		p -= 1;
		*p = x;

	} catch (range_error& r) {
		cerr << "Unexpected range_error: " << r.what() << "!\n";
		return false;
	}

	return expected == 2;
}

static bool do_test3() {
	test3<CheckedPtr<int>, int>	int_test;
	test3<CheckedPtr<X>, X>		X_test;

	return do_one_test("int", 2, int_test) && do_one_test("X", 2, X_test);
}

/********************************************************************************************//**
 * exercise and test CheckedPtr<T>
 ************************************************************************************************/
int main() {
	bool okay = true;

	if (!do_test1())	okay = false;
	if (!do_test2())	okay = false;
	if (!do_test3())	okay = false;

	if (okay)
		cout << "All tests passed!" << endl;

	return okay? EXIT_SUCCESS : EXIT_FAILURE;
}

