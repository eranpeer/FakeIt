FakeIt
======

[![Join the chat at https://gitter.im/eranpeer/FakeIt](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/eranpeer/FakeIt?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[Release 2](https://github.com/eranpeer/FakeIt/releases/latest) is out!
Old style mocking is not supported any more!!

Not supported:
```c++
When(mock[&SomeInterface::foo]).Return(1); 
```
New style (usnig macros) must be used:
```c++
When(Method(mock,foo)).Return(1);
When(OverloadedMethod(mock,foo, int(int) )).Return(1); // For overloaded method
```

GCC: [![Build Status GCC](https://travis-ci.org/eranpeer/FakeIt.svg?branch=master)](https://travis-ci.org/eranpeer/FakeIt)
[![Coverage Status](https://coveralls.io/repos/eranpeer/FakeIt/badge.png?branch=master)](https://coveralls.io/r/eranpeer/FakeIt?branch=master)

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

Download the [Latest Release](https://github.com/eranpeer/FakeIt/releases/latest) and start using FakeIt now!

## Features
* Very simple API based on the expressiveness of C++11.
* Supports all major compilers: GCC, Clang and MSC++.
* Easily integrated with [**GTest**](https://code.google.com/p/googletest/), [**MS Test**](http://en.wikipedia.org/wiki/Visual_Studio_Unit_Testing_Framework) and [**Boost Test**](http://www.boost.org/doc/libs/1_56_0/libs/test/doc/html/index.html).
* Expressive [Arrange-Act-Assert](http://xp123.com/articles/3a-arrange-act-assert/) syntax.
* Create mock classes or **spy existing objects** instantly in one simple line.
* No limitation on number of method arguments.
* Supports dynamic casting.

## Installation
FakeIt is a template library. It does not require any installation. All you need to do is to download the source files and add the "include" folder and the configuration folder of your choice to the include path of your project.
For example:
* To use fakeit with **Google Test** add the "include" folder and the "config/gtest" folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/gtest"
```
* To use fakeit with **MS Test** add the "include" folder and the "config/mstest" folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/mstest"
```
* To use fakeit with **Boost Test** add the "include" folder and the "config/boost" folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/boost"
```
* To use fakeit with **tpunit** add the "include" folder and the "config/tpunit" folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/tpunit"
```
* To use fakeit without any testing framework integration (**standalone**) add the "include" folder and the "config/standalone" folder to the include path of your test project:
```
-I"<fakeit_folder>/include" -I"<fakeit_folder>/config/standalone"
```
It is recommended to build and run the unit tests to make sure FakeIt fits your environment.
### Building and Running the Unit Tests with GCC
```
cd build
make all
```
run the tests by typing
```
./fakit_tests.exe
```

### Building and Running the Unit Tests with Visual Studio 
Open the tests/all_tests.vcxproj project file with Visual Studio 2013. Build and run the project and check the test results. 

### Pulling FakeIt form [**Biicode**](https://www.biicode.com/eranpeer/FakeIt)
FakeIt is published on [**Biicode**](https://www.biicode.com/eranpeer/FakeIt).
To use FakeIt from Biicode simply make the following changes to the biicode.conf file of your project:
* Add the following line to the [Requirements] section:
```
eranpeer/FakeIt: 0 
```
* Add ONE of the following lines to the [includes] section:
```
fakeit.hpp: eranpeer/FakeIt/config/gtest       # To use fakeit with gtest
fakeit.hpp: eranpeer/FakeIt/config/boost       # To use fakeit with boost test
fakeit.hpp: eranpeer/FakeIt/config/mstest      # To use fakeit with mstest
fakeit.hpp: eranpeer/FakeIt/config/tpunit      # To use fakeit with tpunit
fakeit.hpp: eranpeer/FakeIt/config/standalone  # To use fakeit with standalone configuration.
```

## Limitations
* Currently only GCC, Clang and MSC++ are supported.
* Can't mock classes with multiple inheritance.
* Can't mock classes with virtual inheritance.
* Currently mocks are not thread safe. 
