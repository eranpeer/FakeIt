FakeIt
======
 
[![Join the chat at https://gitter.im/eranpeer/FakeIt](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/eranpeer/FakeIt?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Linux / GCC: [![Build status Linux/GCC](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_gcc.yml/badge.svg)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_gcc.yml)
[![Coverage Status](https://coveralls.io/repos/github/eranpeer/FakeIt/badge.svg?branch=master)](https://coveralls.io/github/eranpeer/FakeIt?branch=master)

Linux / Clang: [![Build status Linux/Clang](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_clang.yml/badge.svg)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_clang.yml)

MSVC: [![Build status Windows/MSVC](https://github.com/eranpeer/FakeIt/actions/workflows/ci_windows_msvc.yml/badge.svg)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_windows_msvc.yml)

FakeIt is a simple mocking framework for C++. It supports GCC, Clang and MS Visual C++.

FakeIt is written in C++11 and can be used for testing both C++11 and C++ projects.

```cpp
struct SomeInterface {
    virtual int foo(int) = 0;
    virtual int bar(string) = 0;
};
```

```cpp
// Instantiate a mock object.
Mock<SomeInterface> mock;

// Setup mock behavior.
When(Method(mock,foo)).Return(1); // Method mock.foo will return 1 once.

// Fetch the mock instance.
SomeInterface &i = mock.get();

// Will print "1". 
cout << i.foo(0);
```

Verify method invocation:

```cpp
Mock<SomeInterface> mock;
		
When(Method(mock,foo)).Return(0);

SomeInterface &i = mock.get();

// Production code
i.foo(1);

// Verify method mock.foo was invoked.
Verify(Method(mock,foo));

// Verify method mock.foo was invoked with specific arguments.
Verify(Method(mock,foo).Using(1));
```

Checkout the [Quickstart](https://github.com/eranpeer/FakeIt/wiki/Quickstart) for many more examples!

The master branch has the stable version of FakeIt. Include the most suitable single header in your test project and you are good to go.

## Features
* Packaged as a **single header file**.
* Very simple API based on the expressiveness of C++11.
* Supports all major compilers: GCC, Clang and MSC++.
* Easily integrated with [**GTest**](https://code.google.com/p/googletest/), [**MS Test**](http://en.wikipedia.org/wiki/Visual_Studio_Unit_Testing_Framework) and [**Boost Test**](http://www.boost.org/doc/libs/1_56_0/libs/test/doc/html/index.html).
* Expressive [Arrange-Act-Assert](http://xp123.com/articles/3a-arrange-act-assert/) syntax.
* Create mock classes or **spy existing objects** instantly in one simple line.
* No limitation on number of method arguments.
* Supports dynamic casting.

## Installation
FakeIt is a header only framework. It does not require any installation. For extra simplicity fakeit is packaged as a single header file.

FakeIt is pre-configured to work with some of the major unit testing frameworks. A pre-configured version will use the assertions mechanism of the unit testing framework to integrate the generated error messages into the unit testing framework output.

If you don't find your unit testing framework on the list, simply use the *standalone* configuration.

### Using a pre-packaged single header file
Pre-packaged single header versions of FakeIt are located under the *single_header* folder.
Depending on the unit testing framework you use, simply add one of the pre-packaged versions to the include path of your test project:

* <fakeit_folder>/single\_header/[gtest](https://github.com/google/googletest)
* <fakeit_folder>/single\_header/mstest
* <fakeit_folder>/single\_header/boost
* <fakeit_folder>/single\_header/[catch](https://github.com/catchorg/Catch2) - (v2 and v3)
* <fakeit_folder>/single\_header/[tpunit](https://github.com/tpounds/tpunitpp)
* <fakeit_folder>/single\_header/[mettle](https://github.com/jimporter/mettle)
* <fakeit_folder>/single\_header/qtest
* <fakeit_folder>/single\_header/nunit - (See caveats in config/nunit/fakeit\_instance.hpp)
* <fakeit_folder>/single\_header/[cute](https://github.com/PeterSommerlad/CUTE)
* <fakeit_folder>/single\_header/[doctest](https://github.com/doctest/doctest)
* <fakeit_folder>/single\_header/standalone - (if you don't use a unit testing framework)

For example, to use fakeit with **Google Test** simply add the *single_header/gtest* folder to the include path of your test project:
```
-I"<fakeit_folder>/single_header/gtest"
```

### Using the source header files
Fakeit source code header files are located under the *include* foler. To use FakeIt directly from the source code all you need to do is to download the source files and add the *include* folder and the configuration folder of your choice to the include path of your project.
For example:

* To use fakeit with **Google Test** add the *include* folder and the *config/gtest* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/gtest"
```
* For every other unit test framework (or for using FakeIt without a unit test framework), just replace `gtest` by the name of the configuration:
  * GoogleTest: `gtest`
  * MSTest: `mstest`
  * Boost.Test: `boost`
  * Catch2: `catch`
  * tpunit++: `tpunit`
  * mettle: `mettle`
  * Qt Test: `qtest`
  * NUnit: `nunit`
  * CUTE: `cute`
  * doctest: `doctest`
  * Without a unit test framework: `standalone`
* If you use FakeIt with **NUnit** in a managed Visual Studio C++/CLI project, note that it is useful to define your mocks in `#pragma unmanaged` sections so that you can use lambda expressions.

It is recommended to build and run the unit tests to make sure FakeIt fits your environment.

For GCC, it is recommended to build the test project with -O1 or -O0 flags. Some features of FakeIt may not work with stronger optimizations!!

#### Building and Running the Unit Tests with GCC
```
CC=gcc CXX=g++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```
Run the tests by typing:
```
./build/tests/FakeIt_tests
```

#### Building and Running the Unit Tests with Clang
```
CC=clang CXX=clang++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```
Run the tests by typing:
```
./build/tests/FakeIt_tests
```

#### Building and Running the Unit Tests with Visual Studio 
```
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug -j
```
Run the tests by typing:
```
./build/tests/Debug/FakeIt_tests.exe
```

## Limitations
* Currently only GCC, Clang and MSC++ are supported.
* On GCC, optimization flag O2 and O3 are not supported. You must compile the test project with -O1 or -O0.
* In MSC++, your project must have Edit And Continue debug mode on (https://msdn.microsoft.com/en-us/library/esaeyddf.aspx) which is same of /ZI compiler switch. If you don't use this, you will have exceptions mocking destructors (which includes unique_ptr and other smart pointers). 
* Can't mock classes with multiple inheritance.
* Can't mock classes with virtual inheritance.
* Currently mocks are not thread safe. 
