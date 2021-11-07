# CheckedPtr<T> A Runtime checked pointer for an array of T's

# Synopsis

    template<class T> class CheckedPtr<T>;
    X a[N];                             // Array of X's to bind to
    CheckedPtr<T> p(a, a, a+N);         // Bind to a, position at 1st element
    *p++ = v;
    p[i] = v;

# Description

Run-time range checked pointer class template. Once bound to an array, a 
CheckedPtr<T> maintians a position with in the array, and the upper and lower 
bounds of the array. Each attempt to dereference a CheckedPtr<T> results in a
range-check of the current position, throwing a range_error exception if the 
check fails.

From The C++ Programming Language, 
- 2nd Edition; 7.10, 7.14[19 and 9.9[2]
- 3rd Edition; 11.10, 11.14[19], 19.3, 13.6.3
- 4th Edition; 19.2.4, 27.2.2

# History

Orignally created for a job interview with Rogue Wave Software in 1996, and 
Microsoft Visual C++ v9.0, 10.0 and 10.1. That version was lost, but recreated
from listings and tested with GNU C++ 9.3.0 and CLang++ 10.0.0.

# Author

Randy Merkel, 
Slowly but Surly Software.
