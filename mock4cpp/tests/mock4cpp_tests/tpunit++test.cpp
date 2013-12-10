/**
 * Copyright (c) 2013 Trevor Pounds <trevor.pounds@gmail.com>
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
#include "tpunit++.hpp"

/**
 * Test tpunit++ internal logic, macros and compiler support.
 */
struct TPUnitPPTest : tpunit::TestFixture
{
   TPUnitPPTest() : tpunit::TestFixture
   (
      BEFORE_CLASS(TPUnitPPTest::before_class),
      BEFORE(TPUnitPPTest::before),
      TEST(TPUnitPPTest::test), TEST(TPUnitPPTest::test),
      TEST(TPUnitPPTest::test), TEST(TPUnitPPTest::test),
      TEST(TPUnitPPTest::test), TEST(TPUnitPPTest::test),
      TEST(TPUnitPPTest::test), TEST(TPUnitPPTest::test),
      TEST(TPUnitPPTest::test_invocations),
      TEST(TPUnitPPTest::test_macros),
      AFTER(TPUnitPPTest::after),
      AFTER_CLASS(TPUnitPPTest::after_class)
   )
   {}

   int __after_num,  __after_class_num,
       __before_num, __before_class_num,
       __test_num;

   void after()        { __after_num++; }
   void after_class()  { __after_class_num++; }
   void before()       { __before_num++; }
   void before_class() { __before_class_num++; }
   void test()         { __test_num++; }

   void test_invocations()
   {
      ASSERT_EQUAL(__after_num,        8);
      ASSERT_EQUAL(__after_class_num,  0);
      ASSERT_EQUAL(__before_num,       9);
      ASSERT_EQUAL(__before_class_num, 1);
      ASSERT_EQUAL(__test_num,         8);
   }

   void test_macros()
   {
      PASS();
      ASSERT_TRUE(true);               EXPECT_TRUE(true);
      ASSERT_FALSE(false);             EXPECT_FALSE(false);
      ASSERT_EQUAL(0, 0);              EXPECT_EQUAL(0, 0);
      ASSERT_NOT_EQUAL(true, false);   EXPECT_NOT_EQUAL(true, false);
      ASSERT_GREATER_THAN(1, 0);       EXPECT_GREATER_THAN(1, 0);
      ASSERT_GREATER_THAN_EQUAL(1, 0); EXPECT_GREATER_THAN_EQUAL(1, 0);
      ASSERT_GREATER_THAN_EQUAL(1, 1); EXPECT_GREATER_THAN_EQUAL(1, 1);
      ASSERT_LESS_THAN(0, 1);          EXPECT_LESS_THAN(0, 1);
      ASSERT_LESS_THAN_EQUAL(0, 1);    EXPECT_LESS_THAN_EQUAL(0, 1);
      ASSERT_LESS_THAN_EQUAL(1, 1);    EXPECT_LESS_THAN_EQUAL(1, 1);
      ASSERT_FLOAT_EQUAL(3.14159265f, 3.141592650000001f); EXPECT_FLOAT_EQUAL(3.14159265f, 3.141592650001f);
      ASSERT_FLOAT_EQUAL(0.0f,        -0.0f);              EXPECT_FLOAT_EQUAL(0.0f,        -0.0f);
      ASSERT_FLOAT_EQUAL(1.77245385,  1.7724538500000001); EXPECT_FLOAT_EQUAL(1.77245385,  1.7724538500000001);
      ASSERT_FLOAT_EQUAL(0.0,         -0.0);               EXPECT_FLOAT_EQUAL(0.0,         -0.0);
      ASSERT_FLOAT_NEAR(3.14159265f,  3.1400001f, 0.002f); EXPECT_FLOAT_NEAR(3.14159265f,  3.1400001f, 0.002f); 
      ASSERT_FLOAT_NEAR(1.77245385,   1.77240000, 0.0005); EXPECT_FLOAT_NEAR(1.77245385,   1.77240000, 0.0005); 
      #if defined(TPUNITPP_HAS_EXCEPTIONS)
         int __dummy = 0;
         ASSERT_THROW(throw __dummy, int); EXPECT_THROW(throw __dummy, int);
         ASSERT_NO_THROW(__dummy = 1);     EXPECT_NO_THROW(__dummy = 1);
         ASSERT_ANY_THROW(throw __dummy);  EXPECT_ANY_THROW(throw __dummy);
      #endif
   }
} __TPUnitPPTest;
