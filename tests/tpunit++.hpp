/**
 * Copyright (c) 2013 Trevor Pounds <trevor.pounds@gmail.com>
 *
 * Permission is hereby granted, free of charge, to Any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __TPUNITPP_HPP__
#define __TPUNITPP_HPP__
#include <stdio.h>
/**
 * Declare printf dependency inline to workaround
 * potential #include <stdio.h> compiler/linker bugs.
 */
//extern "C" int printf(const char*, ...);
/**
 * TPUNITPP_VERSION macro contains an integer represented by
 * the value (M*1000000 + N*1000 + P) where M is the major
 * version, N is the minor version, and P is the patch version.
 *
 * TPUNITPP_VERSION_MAJOR is an integer of the major version.
 * TPUNITPP_VERSION_MINOR is an integer of the minor version.
 * TPUNITPP_VERSION_PATCH is an integer of the patch version.
 */
#define TPUNITPP_VERSION 1001000
#define TPUNITPP_VERSION_MAJOR 1
#define TPUNITPP_VERSION_MINOR 1
#define TPUNITPP_VERSION_PATCH 0

/**
 * ABORT(); generates a failure, immediately returning from the
 * currently executing test function.
 * FAIL(); generates a failure, allowing the currently executing
 * test function to continue.
 * PASS(); does nothing, effectively considered a NOP but may be
 * useful for annotating test cases with their desired intent.
 * TRACE(message); adds a trace to the test output with a user
 * specified string message.
 */
#define ABORT() __assert(__FILE__, __LINE__); return;
#define FAIL()  __assert(__FILE__, __LINE__);
#define PASS()  /* do nothing */
#define TRACE(message) __trace(__FILE__, __LINE__, message);

/**
 * The set of core macros for basic predicate testing of boolean
 * expressions and value comparisons.
 *
 * ASSERT_*(...); generates a failure, immediately returning from
 * the currently executing test function if the supplied predicate
 * is not satisfied.
 * EXPECT_*(...); generates a failure, allowing the currently
 * executing test function to continue if the supplied predicate
 * is not satisified.
 */
#define ASSERT_TRUE(condition) if(condition) { PASS(); } else { ABORT(); }
#define EXPECT_TRUE(condition) if(condition) { PASS(); } else { FAIL(); }
#define ASSERT_FALSE(condition) if(condition) { ABORT(); } else { PASS(); }
#define EXPECT_FALSE(condition) if(condition) { FAIL(); } else { PASS(); }
#define ASSERT_EQUAL(lhs, rhs) if((lhs) == (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_EQUAL(lhs, rhs) if((lhs) == (rhs)) { PASS(); } else { FAIL(); }
#define ASSERT_NOT_EQUAL(lhs, rhs) if((lhs) != (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_NOT_EQUAL(lhs, rhs) if((lhs) != (rhs)) { PASS(); } else { FAIL(); }
#define ASSERT_GREATER_THAN(lhs, rhs) if((lhs) > (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_GREATER_THAN(lhs, rhs) if((lhs) > (rhs)) { PASS(); } else { FAIL(); }
#define ASSERT_GREATER_THAN_EQUAL(lhs, rhs) if((lhs) >= (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_GREATER_THAN_EQUAL(lhs, rhs) if((lhs) >= (rhs)) { PASS(); } else { FAIL(); }
#define ASSERT_LESS_THAN(lhs, rhs) if((lhs) < (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_LESS_THAN(lhs, rhs) if((lhs) < (rhs)) { PASS(); } else { FAIL(); }
#define ASSERT_LESS_THAN_EQUAL(lhs, rhs) if((lhs) <= (rhs)) { PASS(); } else { ABORT(); }
#define EXPECT_LESS_THAN_EQUAL(lhs, rhs) if((lhs) <= (rhs)) { PASS(); } else { FAIL(); }

/**
 * The set of floating-point macros used to compare double/float values.
 *
 * ASSERT|EXPECT_FLOAT_EQUAL(lhs, rhs); generates a failure if the given
 * floating-point values are not within 4 ULPs of each other.
 * ASSERT|EXPECT_FLOAT_NEAR(lhs, rhs, abs_error); generates a failure if
 * the given floating-point values exceed the absolute error.
 */
#define ASSERT_FLOAT_EQUAL(lhs, rhs) if(__fp_equal(lhs, rhs, 4)) { PASS(); } else { ABORT(); }
#define EXPECT_FLOAT_EQUAL(lhs, rhs) if(__fp_equal(lhs, rhs, 4)) { PASS(); } else { FAIL(); }
#define ASSERT_FLOAT_NEAR(lhs, rhs, abs_error) if((((lhs) > (rhs)) ? (lhs) - (rhs) : (rhs) - (lhs)) <= (abs_error)) { PASS(); } else { ABORT(); }
#define EXPECT_FLOAT_NEAR(lhs, rhs, abs_error) if((((lhs) > (rhs)) ? (lhs) - (rhs) : (rhs) - (lhs)) <= (abs_error)) { PASS(); } else { FAIL(); }

/**
 * The set of macros for checking whether a statement will throw or not
 * throw an exception. Note, the checked exception macros will generally
 * not work with compilers that do not support exceptions or have them
 * explicitly turned off using a compiler flag (e.g. -fno-exceptions).
 *
 * ASSERT|EXPECT_THROW(statement, exception); generates a failure if
 * the given statement does not throw the supplied excetion.
 * ASSERT|EXPECT_NO_THROW(statement, exception); generates a failure
 * if the given statement throws Any exception. Useful for ensuring
 * a statement never throws an exception.
 * ASSERT|EXPECT_ANY_THROW(statement); generates a failure if the
 * given statement does not throw Any exceptions.
 */
#define ASSERT_THROW(statement, exception) try { statement; ABORT(); } catch(const exception&) { PASS(); } catch(...) { ABORT(); }
#define EXPECT_THROW(statement, exception) try { statement; FAIL(); } catch(const exception&) { PASS(); } catch(...) { FAIL(); }
#define ASSERT_NO_THROW(statement) try { statement; PASS(); } catch(...) { ABORT(); }
#define EXPECT_NO_THROW(statement) try { statement; PASS(); } catch(...) { FAIL(); }
#define ASSERT_ANY_THROW(statement) try { statement; ABORT(); } catch(...) { PASS(); }
#define EXPECT_ANY_THROW(statement) try { statement; FAIL(); } catch(...) { PASS(); }

/**
 * The set of convenience macros for registering functions with the test
 * fixture.
 *
 * AFTER(function); registers a function to run once after each subsequent
 * test function within a test fixture.
 * AFTER_CLASS(function); registers a function to run once after all test
 * functions within a test fixture. Useful for cleaning up shared state
 * used by all test functions.
 * BEFORE(function); registers a function to run once before each subsequent
 * test function within a test fixture.
 * BEFORE_CLASS(function); registers a function to run once before all test
 * functions within a test fixture. Useful for initializing shared state
 * used by all test functions.
 * TEST(function); registers a function to run as a test within a test fixture.
 */
#define AFTER(M)        After(&M, "After: " #M)
#define AFTER_CLASS(M)  AfterClass(&M, "AfterClass: " #M)
#define BEFORE(M)       Before(&M, "Before: " #M)
#define BEFORE_CLASS(M) BeforeClass(&M, "BeforeClass: " #M)
#define TEST(M)         Test(&M, #M)

/**
 * Try our best to detect compiler support for exception handling so
 * we can catch and report Any unhandled exceptions as normal failures.
 */
#ifndef TPUNITPP_HAS_EXCEPTIONS
#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#include <exception>
#define TPUNITPP_HAS_EXCEPTIONS 1
#endif
#endif

namespace tpunit {
/**
 * The primary class that provides the integration point for creating user
 * defined test cases. To get started one only needs to derive from TestFixture,
 * define a few test methods and register them with the base constructor.
 */
class TestFixture {
private:

	/**
	 * An internal class representing a TestFixture class.
	 */
	struct method {
		method(TestFixture* obj, void (TestFixture::*addr)(), const char* name, unsigned char type) :
		_this(obj), _addr(addr), _name(), _type(type), _next(0) {
			char* dest = _name;
			while (name && *name != 0) {
				*dest++ = *name++;
			}
			dest = 0;
		}

		~method() {
			delete _next;
		}

		TestFixture* _this;
		void (TestFixture::*_addr)();
		char _name[256];

		enum {
			AFTER_METHOD, AFTER_CLASS_METHOD, BEFORE_METHOD, BEFORE_CLASS_METHOD, TEST_METHOD
		};
		unsigned char _type;

		method* _next;
	};

	/**
	 * An internal class representing a TestFixture class.
	 */
	struct fixture {
		fixture() :
				_afters(0), _after_classes(0), _befores(0), _before_classes(0), _tests(0), _next(0) {
		}

		~fixture() {
			delete _afters;
			delete _after_classes;
			delete _befores;
			delete _before_classes;
			delete _tests;
			delete _next;
		}

		method* _afters;
		method* _after_classes;
		method* _befores;
		method* _before_classes;
		method* _tests;

		fixture* _next;
	};

	/**
	 * A struct holding test statistics. 
	 */
	struct stats {
		stats() :
				_assertions(0), _exceptions(0), _failures(0), _passes(0), _traces(0) {
		}

		int _assertions;
		int _exceptions;
		int _failures;
		int _passes;
		int _traces;
	};

public:

	/**
	 * The base constructor of all test fixtures used to register methods executed by the default runner.
	 *
	 * @param[in] m0...m29 A list of methods to register with the test fixture.
	 */
	TestFixture(method* m0, method* m1 = 0, method* m2 = 0, method* m3 = 0, method* m4 = 0, method* m5 = 0, method* m6 = 0, method* m7 = 0,
			method* m8 = 0, method* m9 = 0, method* m10 = 0, method* m11 = 0, method* m12 = 0, method* m13 = 0, method* m14 = 0,
			method* m15 = 0, method* m16 = 0, method* m17 = 0, method* m18 = 0, method* m19 = 0, method* m20 = 0, method* m21 = 0,
			method* m22 = 0, method* m23 = 0, method* m24 = 0, method* m25 = 0, method* m26 = 0, method* m27 = 0, method* m28 = 0,
			method* m29 = 0) {
		fixture* f = &__fixtures();
		while (f->_next) {
			f = f->_next;
		}
		f = f->_next = new fixture;

#define SET_FIXTURE_METHOD(M) \
               if(M) \
               { \
                  method** m = 0; \
                  switch(M->_type) \
                  { \
                     case method::AFTER_METHOD:        m = &f->_afters;         break; \
                     case method::AFTER_CLASS_METHOD:  m = &f->_after_classes;  break; \
                     case method::BEFORE_METHOD:       m = &f->_befores;        break; \
                     case method::BEFORE_CLASS_METHOD: m = &f->_before_classes; break; \
                     case method::TEST_METHOD:         m = &f->_tests;          break; \
                  } \
                  while(*m && (*m)->_next) { m = &(*m)->_next; } \
                  (*m) ? (*m)->_next = M : *m = M; \
               }
		SET_FIXTURE_METHOD(m0)
		SET_FIXTURE_METHOD(m1)
		SET_FIXTURE_METHOD(m2)
		SET_FIXTURE_METHOD(m3)
		SET_FIXTURE_METHOD(m4)
		SET_FIXTURE_METHOD(m5)
		SET_FIXTURE_METHOD(m6)
		SET_FIXTURE_METHOD(m7)
		SET_FIXTURE_METHOD(m8)
		SET_FIXTURE_METHOD(m9)
		SET_FIXTURE_METHOD(m10)
		SET_FIXTURE_METHOD(m11)
		SET_FIXTURE_METHOD(m12)
		SET_FIXTURE_METHOD(m13)
		SET_FIXTURE_METHOD(m14)
		SET_FIXTURE_METHOD(m15)
		SET_FIXTURE_METHOD(m16)
		SET_FIXTURE_METHOD(m17)
		SET_FIXTURE_METHOD(m18)
		SET_FIXTURE_METHOD(m19)
		SET_FIXTURE_METHOD(m20)
		SET_FIXTURE_METHOD(m21)
		SET_FIXTURE_METHOD(m22)
		SET_FIXTURE_METHOD(m23)
		SET_FIXTURE_METHOD(m24)
		SET_FIXTURE_METHOD(m25)
		SET_FIXTURE_METHOD(m26)
		SET_FIXTURE_METHOD(m27)
		SET_FIXTURE_METHOD(m28)
		SET_FIXTURE_METHOD(m29)
#undef SET_FIXTURE_METHOD
	}

	/**
	 * Registers a method to run once immediately after each test method registered with the test fixture.
	 *
	 * @param[in] _method A method to register with the test fixture.
	 * @param[in] _name The internal name of the method used when status messages are displayed.
	 */
	template<typename C>
	method* After(void (C::*_method)(), const char* _name) {
		return new method(this, reinterpret_cast<void (TestFixture::*)()>(_method), _name, method::AFTER_METHOD);}

	/**
	 * Registers a method to run once immediately after all after/before/test methods registered with
	 * the test fixture. Useful for cleaning up shared state used by methods in a test fixture.
	 *
	 * @param[in] _method A method to register with the test fixture.
	 * @param[in] _name The internal name of the method used when status messages are displayed.
	 */
	template <typename C>
	method* AfterClass(void (C::*_method)(), const char* _name)
	{	return new method(this, reinterpret_cast<void (TestFixture::*)()>(_method), _name, method::AFTER_CLASS_METHOD);}

	/**
	 * Registers a method to run once immediately before each test method registered with the test fixture.
	 *
	 * @param[in] _method A method to register with the test fixture.
	 * @param[in] _name The internal name of the method used when status messages are displayed.
	 */
	template <typename C>
	method* Before(void (C::*_method)(), const char* _name)
	{	return new method(this, reinterpret_cast<void (TestFixture::*)()>(_method), _name, method::BEFORE_METHOD);}

	/**
	 * Registers a method to run once immediately before all after/before/test methods registered with
	 * the test fixture. Useful for intializing shared state used by methods in a test fixture.
	 *
	 * @param[in] _method A method to register with the test fixture.
	 * @param[in] _name The internal name of the method used when status messages are displayed.
	 */
	template <typename C>
	method* BeforeClass(void (C::*_method)(), const char* _name)
	{	return new method(this, reinterpret_cast<void (TestFixture::*)()>(_method), _name, method::BEFORE_CLASS_METHOD);}

	/**
	 * Registers a method to run as a test with the test fixture.
	 *
	 * @param[in] _method A method to register with the test fixture.
	 * @param[in] _name The internal name of the method used when status messages are displayed.
	 */
	template <typename C>
	method* Test(void (C::*_method)(), const char* _name)
	{	return new method(this, reinterpret_cast<void (TestFixture::*)()>(_method), _name, method::TEST_METHOD);}

protected:

	static int __do_run()
	{
		fixture* f = __fixtures()._next;
		while(f)
		{
			printf("[--------------]\n");
			__do_methods(f->_before_classes);
			__do_tests(f);
			__do_methods(f->_after_classes);
			printf("[--------------]\n\n");
			f = f->_next;
		}
		printf("[==============]\n");
		printf("[ TEST RESULTS ] Passed: %i, Failed: %i\n", __stats()._passes, __stats()._failures);
		printf("[==============]\n");
		return __stats()._failures;
	}

	/**
	 * Determine if two binary32 single precision IEEE 754 floating-point
	 * numbers are equal using unit in the last place (ULP) analysis.
	 *
	 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm 
	 */
	static bool __fp_equal(float lhs, float rhs, unsigned char ulps)
	{
		union
		{
			float f;
			char c[4];
		}lhs_u, rhs_u;
		lhs_u.f = lhs;
		rhs_u.f = rhs;

		bool lil_endian = ((unsigned char) 0x00FF) == 0xFF;
		int msb = lil_endian ? 3 : 0;
		int lsb = lil_endian ? 0 : 3;
		if(lhs_u.c[msb] < 0)
		{
			lhs_u.c[0 ^ lsb] = 0x00 - lhs_u.c[0 ^ lsb];
			lhs_u.c[1 ^ lsb] = (((unsigned char) lhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[1 ^ lsb];
			lhs_u.c[2 ^ lsb] = (((unsigned char) lhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[2 ^ lsb];
			lhs_u.c[3 ^ lsb] = (((unsigned char) lhs_u.c[2 ^ lsb] > 0x00) ? 0x7F : 0x80) - lhs_u.c[3 ^ lsb];
		}
		if(rhs_u.c[msb] < 0)
		{
			rhs_u.c[0 ^ lsb] = 0x00 - rhs_u.c[0 ^ lsb];
			rhs_u.c[1 ^ lsb] = (((unsigned char) rhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[1 ^ lsb];
			rhs_u.c[2 ^ lsb] = (((unsigned char) rhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[2 ^ lsb];
			rhs_u.c[3 ^ lsb] = (((unsigned char) rhs_u.c[2 ^ lsb] > 0x00) ? 0x7F : 0x80) - rhs_u.c[3 ^ lsb];
		}
		return (lhs_u.c[1] == rhs_u.c[1] && lhs_u.c[2] == rhs_u.c[2] && lhs_u.c[msb] == rhs_u.c[msb]) &&
		((lhs_u.c[lsb] > rhs_u.c[lsb]) ? lhs_u.c[lsb] - rhs_u.c[lsb] : rhs_u.c[lsb] - lhs_u.c[lsb]) <= ulps;
	}

	/**
	 * Determine if two binary64 double precision IEEE 754 floating-point
	 * numbers are equal using unit in the last place (ULP) analysis.
	 *
	 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm 
	 */
	static bool __fp_equal(double lhs, double rhs, unsigned char ulps)
	{
		union
		{
			double d;
			char c[8];
		}lhs_u, rhs_u;
		lhs_u.d = lhs;
		rhs_u.d = rhs;

		bool lil_endian = ((unsigned char) 0x00FF) == 0xFF;
		int msb = lil_endian ? 7 : 0;
		int lsb = lil_endian ? 0 : 7;
		if(lhs_u.c[msb] < 0)
		{
			lhs_u.c[0 ^ lsb] = 0x00 - lhs_u.c[0 ^ lsb];
			lhs_u.c[1 ^ lsb] = (((unsigned char) lhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[1 ^ lsb];
			lhs_u.c[2 ^ lsb] = (((unsigned char) lhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[2 ^ lsb];
			lhs_u.c[3 ^ lsb] = (((unsigned char) lhs_u.c[2 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[3 ^ lsb];
			lhs_u.c[4 ^ lsb] = (((unsigned char) lhs_u.c[3 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[4 ^ lsb];
			lhs_u.c[5 ^ lsb] = (((unsigned char) lhs_u.c[4 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[5 ^ lsb];
			lhs_u.c[6 ^ lsb] = (((unsigned char) lhs_u.c[5 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[6 ^ lsb];
			lhs_u.c[7 ^ lsb] = (((unsigned char) lhs_u.c[6 ^ lsb] > 0x00) ? 0x7F : 0x80) - lhs_u.c[7 ^ lsb];
		}
		if(rhs_u.c[msb] < 0)
		{
			rhs_u.c[0 ^ lsb] = 0x00 - rhs_u.c[0 ^ lsb];
			rhs_u.c[1 ^ lsb] = (((unsigned char) rhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[1 ^ lsb];
			rhs_u.c[2 ^ lsb] = (((unsigned char) rhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[2 ^ lsb];
			rhs_u.c[3 ^ lsb] = (((unsigned char) rhs_u.c[2 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[3 ^ lsb];
			rhs_u.c[4 ^ lsb] = (((unsigned char) rhs_u.c[3 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[4 ^ lsb];
			rhs_u.c[5 ^ lsb] = (((unsigned char) rhs_u.c[4 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[5 ^ lsb];
			rhs_u.c[6 ^ lsb] = (((unsigned char) rhs_u.c[5 ^ lsb] > 0x00) ? 0xFF : 0x00) - rhs_u.c[6 ^ lsb];
			rhs_u.c[7 ^ lsb] = (((unsigned char) rhs_u.c[6 ^ lsb] > 0x00) ? 0x7F : 0x80) - rhs_u.c[7 ^ lsb];
		}
		return (lhs_u.c[1] == rhs_u.c[1] && lhs_u.c[2] == rhs_u.c[2] &&
		lhs_u.c[3] == rhs_u.c[3] && lhs_u.c[4] == rhs_u.c[4] &&
		lhs_u.c[5] == rhs_u.c[5] && lhs_u.c[6] == rhs_u.c[6] &&
		lhs_u.c[msb] == rhs_u.c[msb]) &&
		((lhs_u.c[lsb] > rhs_u.c[lsb]) ? lhs_u.c[lsb] - rhs_u.c[lsb] : rhs_u.c[lsb] - lhs_u.c[lsb]) <= ulps;
	}

	static void __assert(const char* _file, int _line)
	{	printf("[              ]    assertion #%i at %s:%i\n", ++__stats()._assertions, _file, _line);}

	static void __exception(const char* _message)
	{	printf("[              ]    exception #%i cause: %s\n", ++__stats()._exceptions, _message);}

	static void __trace(const char* _file, int _line, const char* _message)
	{	printf("[              ]    trace #%i at %s:%i: %s\n", ++__stats()._traces, _file, _line, _message);}

private:

#ifdef TPUNITPP_HAS_EXCEPTIONS
#define __TPUNITPP_TRY      try
#define __TPUNITPP_CATCH(E) catch(E)
#else
#define __TPUNITPP_TRY      if(true)
#define __TPUNITPP_CATCH(E) if(false)
#endif

	static void __do_methods(method* m)
	{
		while(m)
		{
			__TPUNITPP_TRY
			{	(*m->_this.*m->_addr)();}
			__TPUNITPP_CATCH(const std::exception& e)
			{	__exception(e.what());}
			__TPUNITPP_CATCH(...)
			{	__exception("caught unknown exception type");}
			m = m->_next;
		}
	}

	static void __do_tests(fixture* f)
	{
		method* t = f->_tests;
		while(t)
		{
			__do_methods(f->_befores);

			int _prev_assertions = __stats()._assertions;
			int _prev_exceptions = __stats()._exceptions;
			printf("[ RUN          ] %s\n", t->_name);
			__TPUNITPP_TRY
			{	(*t->_this.*t->_addr)();}
			__TPUNITPP_CATCH(const std::exception& e)
			{	__exception(e.what());}
			__TPUNITPP_CATCH(...)
			{	__exception("caught unknown exception type");}
			if(_prev_assertions == __stats()._assertions &&
			_prev_exceptions == __stats()._exceptions)
			{
				printf("[       PASSED ] %s\n", t->_name);
				__stats()._passes++;
			}
			else
			{
				printf("[       FAILED ] %s\n", t->_name);
				__stats()._failures++;
			}
			t = t->_next;

			__do_methods(f->_afters);
		}
	}

	static stats& __stats()
	{
		static stats _stats;
		return _stats;
	}

	static fixture& __fixtures()
	{
		static fixture _fixtures;
		return _fixtures;
	}
};

/**
 * A class containing the primary entry point for running all registered
 * tpunit++ test cases. Generally this class is wrapped by the user's
 * main function.
 */
class Tests: private TestFixture {
public:

	/**
	 * Run all of the registered test cases and return the number of failed assertions.
	 *
	 * @return The number of failing assertions. (e.g. zero if all tests pass, otherwise non-zero)
	 */
	static int Run() {
		return TestFixture::__do_run();
	}

private:

	Tests() :
			TestFixture(0) { /* disable instance creation */
	}
};
} // namespace tpunit
#endif //__TPUNITPP_HPP__
