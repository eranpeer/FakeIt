
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


# Each subdirectory must supply rules for building sources it contributes
%.o: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 --coverage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


