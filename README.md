# CheckedPtr<T> A Runtime checked pointer for an array of T's

# Synopsis

    template<class T> class CheckedPtr<T>;
    X a[N];                                   // Array of X's to bind to
    CheckedPtr<T> p(a, a, N);                 // Bind to a's first element

# Description

Run-time range checked pointer for a T. When bound to an array of T, a
CheckedPtr<T> maintains the start and length of the array, and the current
position in the array. Any attempt to dereference a CheckedPtr<T> results in a
range-check. If the current position is out of range, and range_error
exception is thrown.

From The C++ Programming Language, 
- 2nd Edition; 7.10, 7.14[19 and 9.9[2]
- 3rd Edition; 11.10, 11.14[19], 19.3, 13.6.3
- 4th Edition; 19.2.4, 27.2.2

# Author

Randy Merkel, 
Slowly but Surly Software.
