/********************************************************************************************//**
 * @file xCheckedPtr.cc
 *
 * Test CheckedPtr<T>
 *
 * Formating: 4 column per hard tab.
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
 * CheckedPtr<T> test base
 ************************************************************************************************/
template<class P, class T>
class Test {
public:
	virtual bool operator()() = 0;
};

/********************************************************************************************//**
 * Test CheckedPtr<T>::operator --,+= and *
 ************************************************************************************************/
template<class P, class T>
class test1 : public Test<P, T> {
public:
	bool operator()();
};

template<class P, class T>
bool test1<P, T>::operator()() {
	T x{};
	T a[10];
	P p(a, a, 10);

	try {
		--p;								// underrun test
		try {
			*p = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
		}

		++p;
		*p = x;

		for (int i = 0; i < 10; ++i)		// underrun test
			++p;
		try {
			*p = x;
		} catch (range_error& r) {
			cout << "Expected range_error: " << r.what() << '\n';
		}
		--p;
		*p;

		for (P p(a, a, 10); p != &a[10]; ++p)
			;								// pre-increment test

		for (P p(a, a, 10); p != &a[10]; p++)
			;								// post-increment test

	} catch (range_error& r) {
		cerr << "Unexpected rnage_error: " << r.what() << endl;
		return false;
	}

	return true;
}

/********************************************************************************************//**
 * run one test on one type
 ************************************************************************************************/
template<class P, class T>
bool do_one_test(const char* type, int testn, Test<P, T>& func) {
	cout << "CheckedPtr<T>: <" << type << "> test #" << testn << "..." << '\n';
	bool okay = func();
	cout << '<' << type << "> Test #" << testn << (okay? " Passed " : " Failed!") << '\n';
	cout << '\n';

	return okay;
}

/********************************************************************************************//**
 * do test # n for both int's and X's
 ************************************************************************************************/
static bool do_test_1() {
	test1<CheckedPtr<int>, int>	int_test;
	test1<CheckedPtr<X>, X>		X_test;

	return do_one_test("int", 1, int_test) && do_one_test("X", 1, X_test);
}

/********************************************************************************************//**
 * exercise and test CheckedPtr<T>
 ************************************************************************************************/
int main() {
	bool okay = true;

	if (!do_test_1()) okay = false;

	if (okay)
		cout << "All tests passed!" << endl;

	return okay? EXIT_SUCCESS : EXIT_FAILURE;
}

