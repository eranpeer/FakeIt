FakeIt
======
GCC: [![Build Status GCC](https://travis-ci.org/eranpeer/FakeIt.svg?branch=master)](https://travis-ci.org/eranpeer/FakeIt)
[![Coverage Status](https://coveralls.io/repos/eranpeer/FakeIt/badge.png?branch=master)](https://coveralls.io/r/eranpeer/FakeIt?branch=master)

MSC: [![Build status MSC](https://ci.appveyor.com/api/projects/status/sy2dk8se2yoxaqve)](https://ci.appveyor.com/project/eranpeer/fakeit)

FakeIt is a simple mocking framework for C++11. It supports GCC and MS Visual C++.

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
When(mock[&SomeInterface::foo]).Return(1);

// Fetch the mock instance.
SomeInterface &i = mock.get();

// Will print "1". 
cout << i.foo(0);


```
Verify method invocation
```cpp
Mock<SomeInterface> mock;
		
When(mock[&SomeInterface::foo]).Return(0);

SomeInterface &i = mock.get();

// Production code
i.foo(1);

// Verify foo was invoked
Verify(mock[&SomeInterface::foo]);

// Verify foo was invoked with specific arguments
Verify(mock[&SomeInterface::foo].Using(1));
```

Checkout the [Quickstart](https://github.com/eranpeer/FakeIt/wiki/Quickstart) for many more examples!

Download the [Latest Release](https://github.com/eranpeer/FakeIt/releases/latest) and start using FakeIt now!


## Features
* Very simple API based on the expressiveness of C++11.
* Supports both GCC and MS C++.
* Expressive [Arrange-Act-Assert](http://xp123.com/articles/3a-arrange-act-assert/) syntax.
* Create mock classes or **spy existing objects** instantly in one simple line.
* No limitation on number of method arguments.
* Supports dynamic casting.

## Installation
FakeIt is a template library. It does not require any installation. All you need to do is to download the source files and add the "include" folder to the include path of your project.
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

## Limitations
* Currently only GCC and MS C++ are supported.
* Can't mock classes with multiple inheritance.
* Can't mock classes with virtual inheritance.
* Currently mocks are not thread safe. 
