/**
 * Copyright (c) 2011-2020 Trevor Pounds <trevor.pounds@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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

#include <cstdio>

/**
 * TPUNITPP_VERSION macro contains an integer represented by
 * the value (M*1000000 + N*1000 + P) where M is the major
 * version, N is the minor version, and P is the patch version.
 *
 * TPUNITPP_VERSION_MAJOR is an integer of the major version.
 * TPUNITPP_VERSION_MINOR is an integer of the minor version.
 * TPUNITPP_VERSION_PATCH is an integer of the patch version.
 */
#define TPUNITPP_VERSION 1003000
#define TPUNITPP_VERSION_MAJOR 1
#define TPUNITPP_VERSION_MINOR 3
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
#define ABORT() tpunit_detail_assert(__FILE__, __LINE__); return;
#define FAIL()  tpunit_detail_assert(__FILE__, __LINE__);
#define PASS()  /* do nothing */
#define TRACE(message) tpunit_detail_trace(__FILE__, __LINE__, message);

/**
 * The set of core macros for basic predicate testing of boolean
 * expressions and value comparisons.
 *
 * ASSERT_*(...); generates a failure, immediately returning from
 * the currently executing test function if the supplied predicate
 * is not satisfied.
 * EXPECT_*(...); generates a failure, allowing the currently
 * executing test function to continue if the supplied predicate
 * is not satisfied.
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
#define ASSERT_FLOAT_EQUAL(lhs, rhs) if(tpunit_detail_fp_equal(lhs, rhs, 4)) { PASS(); } else { ABORT(); }
#define EXPECT_FLOAT_EQUAL(lhs, rhs) if(tpunit_detail_fp_equal(lhs, rhs, 4)) { PASS(); } else { FAIL(); }
#define ASSERT_FLOAT_NEAR(lhs, rhs, abs_error) if((((lhs) > (rhs)) ? (lhs) - (rhs) : (rhs) - (lhs)) <= (abs_error)) { PASS(); } else { ABORT(); }
#define EXPECT_FLOAT_NEAR(lhs, rhs, abs_error) if((((lhs) > (rhs)) ? (lhs) - (rhs) : (rhs) - (lhs)) <= (abs_error)) { PASS(); } else { FAIL(); }

/**
 * The set of macros for checking whether a statement will throw or not
 * throw an exception. Note, the checked exception macros will generally
 * not work with compilers that do not support exceptions or have them
 * explicitly turned off using a compiler flag (e.g. -fno-exceptions).
 *
 * ASSERT|EXPECT_THROW(statement, exception); generates a failure if
 * the given statement does not throw the supplied exception.
 * ASSERT|EXPECT_NO_THROW(statement, exception); generates a failure
 * if the given statement throws any exception. Useful for ensuring
 * a statement never throws an exception.
 * ASSERT|EXPECT_ANY_THROW(statement); generates a failure if the
 * given statement does not throw any exceptions.
 */
#define ASSERT_THROW(statement, exception) try { statement; ABORT(); } catch(const exception&) { PASS(); } catch(...) { ABORT(); }
#define EXPECT_THROW(statement, exception) try { statement; FAIL(); } catch(const exception&) { PASS(); } catch(...) { FAIL(); }
#define ASSERT_NO_THROW(statement) try { statement; PASS(); } catch(...) { ABORT(); }
#define EXPECT_NO_THROW(statement) try { statement; PASS(); } catch(...) { FAIL(); }
#define ASSERT_ANY_THROW(statement) try { statement; ABORT(); } catch(...) { PASS(); }
#define EXPECT_ANY_THROW(statement) try { statement; FAIL(); } catch(...) { PASS(); }

/**
 * A macro that can be used to check whether an input matches acceptable
 * values. A matcher implementation is a simple type containing a single
 * boolean function that is applied to the input. The match is considered
 * successful if the function returns true and unsuccessful if it returns
 * false.
 *
 * e.g.
 *
 *    struct AlwaysMatches {
 *       template <typename T>
 *       bool matches(T) { return true; }
 *    };
 *
 *    struct NeverMatches {
 *       template <typename T>
 *       bool matches(T) { return false; }
 *    };
 *
 * ASSERT|EXPECT_THAT(obj, matcher); fail if the matcher evaluates to false.
 */
#define ASSERT_THAT(obj, matcher) if(matcher.matches(obj)) { PASS(); } else { ABORT(); }
#define EXPECT_THAT(obj, matcher) if(matcher.matches(obj)) { PASS(); } else { FAIL(); }

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
#define AFTER(M)        tpunit_detail_method(&M, #M, method::AFTER_METHOD)
#define AFTER_CLASS(M)  tpunit_detail_method(&M, #M, method::AFTER_CLASS_METHOD)
#define BEFORE(M)       tpunit_detail_method(&M, #M, method::BEFORE_METHOD)
#define BEFORE_CLASS(M) tpunit_detail_method(&M, #M, method::BEFORE_CLASS_METHOD)
#define TEST(M)         tpunit_detail_method(&M, #M, method::TEST_METHOD)

/**
 * Try our best to detect compiler support for exception handling so
 * we can catch and report any unhandled exceptions as normal failures.
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
      protected:

         /**
          * Internal class encapsulating a registered test method.
          */
         struct method {
            method(TestFixture* obj, void (TestFixture::*addr)(), const char* name, unsigned char type)
               : _this(obj)
               , _addr(addr)
               , _type(type)
               , _next(0) {
               char* dest = _name;
               while (name && *name != 0) {
                  *dest++ = *name++;
               }
               *dest = 0;
            }

            ~method() {
               delete _next;
            }

            TestFixture* _this;
            void (TestFixture::*_addr)();
            char _name[1024];

            enum {
               AFTER_METHOD,  AFTER_CLASS_METHOD,
               BEFORE_METHOD, BEFORE_CLASS_METHOD,
               TEST_METHOD
            };
            unsigned char _type;

            method* _next;
         };

         /**
          * Internal class encapsulating test statistics.
          */
         struct stats {
            stats()
               : _assertions(0)
               , _exceptions(0)
               , _failures(0)
               , _passes(0)
               , _traces(0)
               {}

            int _assertions;
            int _exceptions;
            int _failures;
            int _passes;
            int _traces;
         };

         /**
          * Base constructor to register methods with the test fixture. A test
          * fixture can register up to 50 methods.
          *
          * @param[in] m0..m49 The methods to register with the test fixture.
          */
         TestFixture(method* m0,      method* m1  = 0, method* m2  = 0, method* m3  = 0, method* m4  = 0,
                     method* m5  = 0, method* m6  = 0, method* m7  = 0, method* m8  = 0, method* m9  = 0,
                     method* m10 = 0, method* m11 = 0, method* m12 = 0, method* m13 = 0, method* m14 = 0,
                     method* m15 = 0, method* m16 = 0, method* m17 = 0, method* m18 = 0, method* m19 = 0,
                     method* m20 = 0, method* m21 = 0, method* m22 = 0, method* m23 = 0, method* m24 = 0,
                     method* m25 = 0, method* m26 = 0, method* m27 = 0, method* m28 = 0, method* m29 = 0,
                     method* m30 = 0, method* m31 = 0, method* m32 = 0, method* m33 = 0, method* m34 = 0,
                     method* m35 = 0, method* m36 = 0, method* m37 = 0, method* m38 = 0, method* m39 = 0,
                     method* m40 = 0, method* m41 = 0, method* m42 = 0, method* m43 = 0, method* m44 = 0,
                     method* m45 = 0, method* m46 = 0, method* m47 = 0, method* m48 = 0, method* m49 = 0)
            : _afters(0), _after_classes(0), _befores(0), _before_classes(0), _tests(0), _next(0)
         {
            TestFixture** f = tpunit_detail_fixtures();
            if (*f) {
               while ((*f)->_next) {
                  f = &((*f)->_next);
               }
               (*f)->_next = this;
            } else {
               *f = this;
            }

            method* methods[50] = { m0,  m1,  m2,  m3,  m4,  m5,  m6,  m7,  m8,  m9,
                                    m10, m11, m12, m13, m14, m15, m16, m17, m18, m19,
                                    m20, m21, m22, m23, m24, m25, m26, m27, m28, m29,
                                    m30, m31, m32, m33, m34, m35, m36, m37, m38, m39,
                                    m40, m41, m42, m43, m44, m45, m46, m47, m48, m49 };

            for (int i = 0; i < 50; i++) {
               if (methods[i]) {
                  method** m = 0;
                  switch(methods[i]->_type) {
                     case method::AFTER_METHOD:        m = &_afters;         break;
                     case method::AFTER_CLASS_METHOD:  m = &_after_classes;  break;
                     case method::BEFORE_METHOD:       m = &_befores;        break;
                     case method::BEFORE_CLASS_METHOD: m = &_before_classes; break;
                     case method::TEST_METHOD:         m = &_tests;          break;
                  }
                  while (*m && (*m)->_next) {
                     m = &(*m)->_next;
                  }
                  (*m) ? (*m)->_next = methods[i] : *m = methods[i];
               }
            }
         }

         ~TestFixture() {
            delete _afters;
            delete _after_classes;
            delete _befores;
            delete _before_classes;
            delete _tests;
         }

         /**
          * Create a new method to register with the test fixture.
          *
          * @param[in] _method A method to register with the test fixture.
          * @param[in] _name The internal name of the method used when status messages are displayed.
          */
         template <typename C>
         method* tpunit_detail_method(void (C::*_method)(), const char* _name, unsigned char _type) {
            return new method(this, static_cast<void (TestFixture::*)()>(_method), _name, _type);
         }

         /**
          * Determine if two binary32 single precision IEEE 754 floating-point
          * numbers are equal using unit in the last place (ULP) analysis.
          *
          * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm 
          */
         static bool tpunit_detail_fp_equal(float lhs, float rhs, unsigned char ulps) {
            union {
               float f;
               char  c[4];
            } lhs_u, rhs_u;
            lhs_u.f = lhs;
            rhs_u.f = rhs;

            bool lil_endian = ((unsigned char) 0x00FF) == 0xFF;
            int msb = lil_endian ? 3 : 0;
            int lsb = lil_endian ? 0 : 3;
            if (lhs_u.c[msb] < 0) {
               lhs_u.c[0 ^ lsb] = 0x00 - lhs_u.c[0 ^ lsb];
               lhs_u.c[1 ^ lsb] = (((unsigned char) lhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[1 ^ lsb];
               lhs_u.c[2 ^ lsb] = (((unsigned char) lhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[2 ^ lsb];
               lhs_u.c[3 ^ lsb] = (((unsigned char) lhs_u.c[2 ^ lsb] > 0x00) ? 0x7F : 0x80) - lhs_u.c[3 ^ lsb];
            }
            if (rhs_u.c[msb] < 0) {
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
         static bool tpunit_detail_fp_equal(double lhs, double rhs, unsigned char ulps) {
            union {
               double d;
               char   c[8];
            } lhs_u, rhs_u;
            lhs_u.d = lhs;
            rhs_u.d = rhs;

            bool lil_endian = ((unsigned char) 0x00FF) == 0xFF;
            int msb = lil_endian ? 7 : 0;
            int lsb = lil_endian ? 0 : 7;
            if (lhs_u.c[msb] < 0) {
               lhs_u.c[0 ^ lsb] = 0x00 - lhs_u.c[0 ^ lsb];
               lhs_u.c[1 ^ lsb] = (((unsigned char) lhs_u.c[0 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[1 ^ lsb];
               lhs_u.c[2 ^ lsb] = (((unsigned char) lhs_u.c[1 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[2 ^ lsb];
               lhs_u.c[3 ^ lsb] = (((unsigned char) lhs_u.c[2 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[3 ^ lsb];
               lhs_u.c[4 ^ lsb] = (((unsigned char) lhs_u.c[3 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[4 ^ lsb];
               lhs_u.c[5 ^ lsb] = (((unsigned char) lhs_u.c[4 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[5 ^ lsb];
               lhs_u.c[6 ^ lsb] = (((unsigned char) lhs_u.c[5 ^ lsb] > 0x00) ? 0xFF : 0x00) - lhs_u.c[6 ^ lsb];
               lhs_u.c[7 ^ lsb] = (((unsigned char) lhs_u.c[6 ^ lsb] > 0x00) ? 0x7F : 0x80) - lhs_u.c[7 ^ lsb];
            }
            if (rhs_u.c[msb] < 0) {
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

         static void tpunit_detail_assert(const char* _file, int _line) {
            printf("[              ]    assertion #%i at %s:%i\n", ++tpunit_detail_stats()._assertions, _file, _line);
         }

         static void tpunit_detail_exception(method* _method, const char* _message) {
            printf("[              ]    exception #%i from %s with cause: %s\n", ++tpunit_detail_stats()._exceptions, _method->_name, _message);
         }

         static void tpunit_detail_trace(const char* _file, int _line, const char* _message) {
            printf("[              ]    trace #%i at %s:%i: %s\n", ++tpunit_detail_stats()._traces, _file, _line, _message);
         }

         static int tpunit_detail_do_run() {
            TestFixture* f = *tpunit_detail_fixtures();
            while (f) {
               printf("[--------------]\n");
               tpunit_detail_do_methods(f->_before_classes);
               tpunit_detail_do_tests(f);
               tpunit_detail_do_methods(f->_after_classes);
               printf("[--------------]\n\n");
               f = f->_next;
            }
            printf("[==============]\n");
            printf("[ TEST RESULTS ] Passed: %i, Failed: %i\n", tpunit_detail_stats()._passes, tpunit_detail_stats()._failures);
            printf("[==============]\n");
            return tpunit_detail_stats()._failures;
         }

      private:

         static void tpunit_detail_do_method(method* m) {
            #ifdef TPUNITPP_HAS_EXCEPTIONS
            try {
            #endif
               (*m->_this.*m->_addr)();
            #ifdef TPUNITPP_HAS_EXCEPTIONS
            } catch(const std::exception& e) {
               tpunit_detail_exception(m, e.what());
            } catch(...) {
               tpunit_detail_exception(m, "caught unknown exception type");
            }
            #endif
         }

         static void tpunit_detail_do_methods(method* m) {
            while (m) {
               tpunit_detail_do_method(m);
               m = m->_next;
            }
         }

         static void tpunit_detail_do_tests(TestFixture* f) {
            method* t = f->_tests;
            while (t) {
               int _prev_assertions = tpunit_detail_stats()._assertions;
               int _prev_exceptions = tpunit_detail_stats()._exceptions;
               printf("[ RUN          ] %s\n", t->_name);
               tpunit_detail_do_methods(f->_befores);
               tpunit_detail_do_method(t);
               tpunit_detail_do_methods(f->_afters);
               if (_prev_assertions == tpunit_detail_stats()._assertions &&
                  _prev_exceptions == tpunit_detail_stats()._exceptions) {
                  printf("[       PASSED ] %s\n", t->_name);
                  tpunit_detail_stats()._passes++;
               } else {
                  printf("[       FAILED ] %s\n", t->_name);
                  tpunit_detail_stats()._failures++;
               }
               t = t->_next;
            }
         }

         static stats& tpunit_detail_stats() {
            static stats _stats;
            return _stats;
         }

         static TestFixture** tpunit_detail_fixtures() {
            static TestFixture* _fixtures = 0;
            return &_fixtures;
         }

         method* _afters;
         method* _after_classes;
         method* _befores;
         method* _before_classes;
         method* _tests;

         TestFixture* _next;
   };

   /**
    * Convenience class containing the entry point to run all registered tests.
    */
   struct Tests : TestFixture {
      /**
       * Run all registered test cases and return the number of failed assertions.
       *
       * @return Number of failed assertions or zero if all tests pass.
       */
      static int run() {
         return tpunit_detail_do_run();
      }
   };
} // namespace tpunit
#endif //__TPUNITPP_HPP__
