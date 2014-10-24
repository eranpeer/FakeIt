#pragma once

#include "config/standalone/StandaloneFakeit.hpp"

static fakeit::DefaultFakeit& Fakeit = fakeit::StandaloneFakeit::getInstance();
