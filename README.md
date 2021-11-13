# CheckedPtr<T> A Runtime checked pointer

# Synopsis

    template<class T> class CheckedPtr<T>;
    X a[N];                             // Array of X's to bind to
    CheckedPtr<T> p(a, a, a+N);         // Bind to a, position at 1st element
    *p++ = v;
    v = p[i];

# Description

A Run-time, range checked pointer, class template for objects of type T. Once
bound to an object, or array of objects, CheckedPtr<T>'s maintains the upper
and lower bounds of of the object(s), as well as a current position. Each
attempt to dereference a CheckedPtr<T> is range-checked, throwing a range_error
exception if the reference is out of bounds.

The default constructor results in a unbound pointer that will throw
range_errors on every dereference. Unbound pointers can be bound to an object,
or arrary of objects, via assignment.

From The C++ Programming Language, 
- 2nd Edition; 7.10, 7.14[19 and 9.9[2]
- 3rd Edition; 11.10, 11.14[19], 19.3, 13.6.3
- 4th Edition; 19.2.4, 27.2.2

# History

Orignally created for a job interview with Rogue Wave Software in 1996, and 
Microsoft Visual C++ v9.0, 10.0 and 10.1. That version was lost, but recreated
from listings and tested with GNU C++ 9.3.0 and CLang++ 10.0.0.

# Author

Randy Merkel, Slowly but Surly Software.
