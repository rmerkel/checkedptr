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
 ************************************************************************************************/
template<class P, class T>
struct test {
	virtual bool operator()() = 0;
};

/********************************************************************************************//**
 ************************************************************************************************/

template<class P, class T>
class test1 : public test<P, T> {
public:
	bool operator()() {
		T a[10];
		static T x;

		try {
			P p(a, a, 10);
			--p;								// underrun test
			try {
				*p = x;
			} catch (range_error& r) {
				cout << "Expected range_error: " << r.what() << '\n';
			}
			++p;
			*p = x;

			// ...

		} catch (range_error& r) {
			cerr << "Unexpected rnage_error: " << r.what() << endl;
			return false;
		}
	
		return true;
	}
};

/********************************************************************************************//**
 ************************************************************************************************/
template<class P, class T>
bool do_one_test(const char* type, int testn, test<P, T>& func) {
	cout << "CheckedPtr<T>: <" << type << "> test #" << testn << "..." << '\n';
	bool okay = func();
	cout << '<' << type << "> Test #" << testn << (okay? " Passed " : " Failed!") << '\n';
	cout << '\n';

	return okay;
}

/********************************************************************************************//**
 ************************************************************************************************/
static bool do_test_1() {
	test1<CheckedPtr<int>, int>	int_test;
//	test1<CheckedPtr<X>, X>		X_test;

	return do_one_test("int", 1, int_test);
}

/********************************************************************************************//**
 ************************************************************************************************/
int main() {
	bool okay = true;

	if (!do_test_1()) okay = false;

	if (okay)
		cout << "All tests passed!" << endl;

	return okay? EXIT_SUCCESS : EXIT_FAILURE;
}

