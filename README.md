# CheckedPtr<T> A Runtime checked pointer for an array of T's

# Synopsis

    template<class T> class CheckedPtr<T>;
    X a[N];                                   // Array of X's to bind to
    CheckedPtr<T> p(a, a, N);                 // Bind to a's first element

# Description

Run-time range checked pointer for a T. Once bound to an array, a 
CheckedPtr<T> maintians a position with in the array, and the upper and lower 
bounds of the array. Each attempt to dereference a CheckedPtr<T> results in a
range-check of the current position, throwing a range_error exception if the 
check fails.

From The C++ Programming Language, 
- 2nd Edition; 7.10, 7.14[19 and 9.9[2]
- 3rd Edition; 11.10, 11.14[19], 19.3, 13.6.3
- 4th Edition; 19.2.4, 27.2.2

# Author

Randy Merkel, 
Slowly but Surly Software.
