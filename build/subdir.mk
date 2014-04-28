
# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tests/default_behaviore_tests.cpp \
../tests/gcc_stubbing_multiple_values_tests.cpp \
../tests/gcc_type_info_tests.cpp \
../tests/msc_stubbing_multiple_values_tests.cpp \
../tests/msc_type_info_tests.cpp \
../tests/referece_types_tests.cpp \
../tests/sequence_verification_tests.cpp \
../tests/stubbing_tests.cpp \
../tests/tpunit++main.cpp \
../tests/type_info_tests.cpp \
../tests/verification_tests.cpp \
../tests/verification_error_formatting_tests.cpp 

OBJS += \
./default_behaviore_tests.o \
./gcc_stubbing_multiple_values_tests.o \
./gcc_type_info_tests.o \
./msc_stubbing_multiple_values_tests.o \
./msc_type_info_tests.o \
./referece_types_tests.o \
./sequence_verification_tests.o \
./stubbing_tests.o \
./tpunit++main.o \
./type_info_tests.o \
./verification_tests.o \
./verification_error_formatting_tests.o

COVERAGE_OBJS += \
./cover_default_behaviore_tests.o \
./cover_gcc_stubbing_multiple_values_tests.o \
./cover_gcc_type_info_tests.o \
./cover_msc_stubbing_multiple_values_tests.o \
./cover_msc_type_info_tests.o \
./cover_referece_types_tests.o \
./cover_sequence_verification_tests.o \
./cover_stubbing_tests.o \
./cover_tpunit++main.o \
./cover_type_info_tests.o \
./cover_verification_tests.o \
./cover_verification_error_formatting_tests.o

CPP_DEPS += \
./default_behaviore_tests.d \
./gcc_stubbing_multiple_values_tests.d \
./gcc_type_info_tests.d \
./msc_stubbing_multiple_values_tests.d \
./msc_type_info_tests.d \
./referece_types_tests.d \
./sequence_verification_tests.d \
./stubbing_tests.d \
./tpunit++main.d \
./type_info_tests.d \
./verification_tests.d \
./verification_error_formatting_tests.d

