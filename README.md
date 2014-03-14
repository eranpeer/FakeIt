FakeIt
======

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

Checkout the [Quickstart](https://github.com/eranpeer/FakeIt/wiki/Quickstart) for many more exmples!

## Features
* Very simple & API based on the expressiveness of C++11.
* Supports both GCC and MS C++.
* No limitation on number of method arguments.
* Supports dynamic casting.
