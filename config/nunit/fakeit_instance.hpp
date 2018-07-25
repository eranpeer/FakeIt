#pragma once

#include "NUnitFakeit.hpp"

/**
 * The main instance of Fakeit for use from tests.
 */
static fakeit::DefaultFakeit& Fakeit = fakeit::NUnitFakeit::getInstance();

/**
 * There are funny rules in managed C++ about when native code can create static objects. It is a very
 * long story. Short of it is: if you get a crash about atExit not being valid outside of the default
 * domain, then you need to make sure that the function creating the static object is called from a static
 * scope. 
 * This statement fixes that crash for UnknownMethod::instance.
 * Some additional information is here: https://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI
 */
static fakeit::MethodInfo& scopeFixer = fakeit::UnknownMethod::instance();

// NOTE: You may find it highly useful to use a section with #pragma unmanaged when writing mocks because it 
// will allow you to use lamba expressions.
