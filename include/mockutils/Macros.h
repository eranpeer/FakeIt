#ifndef Macros_h__
#define Macros_h__

#ifdef _WIN32
#define THROWS throw(...)
#else
#define THROWS noexcept(false)
#endif

#endif
