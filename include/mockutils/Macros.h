#ifndef Macros_h__
#define Macros_h__

#if defined (_WIN32)
#define THROWS throw(...)
#elif defined (__GNUG__)
#define THROWS noexcept(false)
#endif

#endif
