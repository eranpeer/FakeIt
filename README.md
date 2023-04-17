FakeIt
======
 
[![Join the chat at https://gitter.im/eranpeer/FakeIt](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/eranpeer/FakeIt?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Linux / GCC: [![CI Linux/GCC](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_gcc.yml/badge.svg?branch=master&event=push)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_gcc.yml?query=branch%3Amaster+event%3Apush)
[![Coverage Status](https://coveralls.io/repos/github/eranpeer/FakeIt/badge.svg?branch=master)](https://coveralls.io/github/eranpeer/FakeIt?branch=master)

Linux / Clang: [![CI Linux/Clang](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_clang.yml/badge.svg?branch=master&event=push)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_linux_clang.yml?query=branch%3Amaster+event%3Apush)

MSVC: [![CI Windows/MSVC](https://github.com/eranpeer/FakeIt/actions/workflows/ci_windows_msvc.yml/badge.svg?branch=master&event=push)](https://github.com/eranpeer/FakeIt/actions/workflows/ci_windows_msvc.yml?query=branch%3Amaster+event%3Apush)

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


### Configurations' name

Here's the list of each unit testing framework FakeIt is pre-configured with and the name of the respective configuration to use if you want FakeIt to integrate with them (if your favorite unit testing framework is not on the list, simply use the `standalone` configuration):
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
* For every other unit test framework (or for using FakeIt without a unit test framework), just replace `gtest` by the name of the configuration (see `Configurations' name` section for details).
* If you use FakeIt with **NUnit** in a managed Visual Studio C++/CLI project, note that it is useful to define your mocks in `#pragma unmanaged` sections so that you can use lambda expressions.

It is recommended to build and run the unit tests to make sure FakeIt fits your environment.

For GCC, it is recommended to build the test project with -O1 or -O0 flags. Some features of FakeIt may not work with stronger optimizations!!

### Installing FakeIt with CMake

Clone the repository, `cd` into it then execute the following commands:
```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
cmake --build build
cmake --install build
```

Replace `<INSTALL_DIR>` by the path of the directory where you want to install FakeIt.

Then in your project's `CMakeLists.txt`:
```
find_package(FakeIt REQUIRED)
[...]
target_link_libraries(<YOUR_TARGET> PRIVATE FakeIt::FakeIt-<CONFIG>)
```

Where `<YOUR_TARGET>` is the name of the target you want to link FakeIt with, and `<CONFIG>` is the name of the specific configuration you want to use (see `Configurations' name` section for details).

Then, when you'll invoke CMake's configure command, pass the argument `-DFakeIt_ROOT=<INSTALL_DIR>`, with `<INSTALL_DIR>` being the directory where you installed FakeIt.

### Installing FakeIt through Conan

To use FakeIt with Conan, you'll need a `conanfile.txt` like this: 
```
[requires]
fakeit/<VERSION>

[options]
fakeit:integration=<CONFIG>
```

Replace `<VERSION>` by the version of FakeIt you want to use, and `<CONFIG>` with the name of the configuration you want to use (see `Configurations' name` section for details).

### Installing FakeIt through vcpkg

You can download and install fakeit using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install fakeit

The fakeit port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg) on the vcpkg repository.

## Running the tests

### Building and Running the Unit Tests with GCC
```
CC=gcc CXX=g++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON
cmake --build build -j
```
Run the tests by typing:
```
./build/tests/FakeIt_tests
```

### Building and Running the Unit Tests with Clang
```
CC=clang CXX=clang++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON
cmake --build build -j
```
Run the tests by typing:
```
./build/tests/FakeIt_tests
```

### Building and Running the Unit Tests with Visual Studio 
```
cmake -S . -B build -G "Visual Studio 17 2022" -DENABLE_TESTING=ON
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
