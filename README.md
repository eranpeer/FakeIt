FakeIt
======
 
[![Join the chat at https://gitter.im/eranpeer/FakeIt](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/eranpeer/FakeIt?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

GCC: [![Build Status GCC](https://travis-ci.org/eranpeer/FakeIt.svg?branch=master)](https://travis-ci.org/eranpeer/FakeIt)
[![Coverage Status](https://coveralls.io/repos/github/eranpeer/FakeIt/badge.svg?branch=master)](https://coveralls.io/github/eranpeer/FakeIt?branch=master)

MSC: [![Build status MSC](https://ci.appveyor.com/api/projects/status/sy2dk8se2yoxaqve)](https://ci.appveyor.com/project/eranpeer/fakeit)

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
Verify method invocation
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
* <fakeit_folder>/single\_header/[catch](https://github.com/philsquared/Catch) (Tested with Catch 2.0.1)
* <fakeit_folder>/single\_header/[tpunit](https://github.com/tpounds/tpunitpp)
* <fakeit_folder>/single\_header/[mettle](https://github.com/jimporter/mettle)
* <fakeit_folder>/single\_header/qtest
* <fakeit_folder>/single\_header/nunit - (See caveats in config/nunit/fakeit\_instance.hpp)
* <fakeit_folder>/single\_header/standalone

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
* To use fakeit with **MS Test** add the *include* folder and the *config/mstest* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/mstest"
```
* To use fakeit with **Boost Test** add the *include* folder and the *config/boost* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/boost"
```
* To use fakeit with **Catch** add the *include* folder and the *config/catch* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/catch"
```
* To use fakeit with **tpunit** add the *include* folder and the *config/tpunit* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/tpunit"
```
* To use fakeit with **Mettle** add the *include* folder and the *config/mettle* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/mettle"
```
* To use fakeit with **QTest** add the *include* folder and the *config/qtest* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/qtest"
```
* To use fakeit with **NUnit** in a managed Visual Studio C++/CLI project, add the standalone/nunit folder to your project include path. Note, it is useful to define your mocks 
in `#pragma unmanaged` sections so that you can use lambda expressions.
* To use fakeit without any testing framework integration (**standalone**) add the *include* folder and the *config/standalone* folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/standalone"
```
It is recommended to build and run the unit tests to make sure FakeIt fits your environment.

For GCC, it is recommended to build the test project with -O1 or -O0 flags. Some features of Fakeit may not work with stonger optimizations!!

#### Building and Running the Unit Tests with GCC
```
cd build
make all
```
run the tests by typing
```
./fakeit_tests.exe
```

#### Building and Running the Unit Tests with Clang
```
cd build
make -f clang_makefile all
```
run the tests by typing
```
./fakeit_tests.exe
```

#### Building and Running the Unit Tests with Visual Studio 
Open the tests/all_tests.vcxproj project file with Visual Studio. Build and run the project and check the test results. 

## Limitations
* Currently only GCC, Clang and MSC++ are supported.
* On GCC, optimization flag O2 and O3 are not supported. You must compile the test project with -O1 or -O0.
* In MSC++, your project must have Edit And Continue debug mode on (https://msdn.microsoft.com/en-us/library/esaeyddf.aspx) which is same of /ZI compiler switch. If you don't use this, you will have exceptions mocking destructors (which includes unique_ptr and other smart pointers). 
* Can't mock classes with multiple inheritance.
* Can't mock classes with virtual inheritance.
* Currently mocks are not thread safe. 
