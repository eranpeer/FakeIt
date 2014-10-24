#pragma once

#include "config/tpunit/TpunitFakeit.hpp"

static fakeit::DefaultFakeit& Fakeit = fakeit::TpunitFakeit::getInstance();
