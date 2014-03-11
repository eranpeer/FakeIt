FakeIt
======

FakeIt is a simple mocking framework for c++11. It supports GCC and MS visual C++.

Stubbing methods
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
When(mock[&SomeInterface::foo]).AlwaysReturn(1);

// Fetch the mock instance.
SomeInterface &i = mock.get();

// Will print "1". 
cout << i.foo(0);


```

Verify method invocation
```cpp
Mock<SomeInterface> mock;
		
When(mock[&SomeInterface::foo]).AlwaysReturn(0);
When(mock[&SomeInterface::bar]).AlwaysReturn(0);

SomeInterface &i = mock.get();

// Production code
i.foo(1);
i.bar("some value");

// Verify foo & bar where invoked
Verify(mock[&SomeInterface::foo]);
Verify(mock[&SomeInterface::bar]);

// Verify foo & bar where invoked with specific arguments
Verify(mock[&SomeInterface::foo].Using(1));
Verify(mock[&SomeInterface::bar].Using("some value"));

// Verify foo & bar where never invoked with other arguments
Verify(mock[&SomeInterface::foo].Using(2)).Never();
Verify(mock[&SomeInterface::bar].Using("some other value")).Never();
```
