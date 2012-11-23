#ifndef vgl_public_h
#define vgl_public_h

// Use this macro to mark classes and functions which should be visible to
// users of the library. Usage is like this (in your header files):
//
// For classes:
//    class PUBLIC MyClass { ... };
//
// For functions:
//    PUBLIC void myFunc();

#if defined(_WIN32)
#  define PUBLIC __declspec(dllexport)
#else
#  define PUBLIC __attribute__ ((visibility("default")))
#endif

#endif // vgl_public_h

