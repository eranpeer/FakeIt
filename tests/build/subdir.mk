
# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../demo_dests.cpp \
../gcc_stubbing_multiple_values_tests.cpp \
../gcc_type_info_tests.cpp \
../msc_stubbing_multiple_values_tests.cpp \
../msc_type_info_tests.cpp \
../referece_types_tests.cpp \
../sequence_verification_tests.cpp \
../stubbing_tests.cpp \
../tpunit++main.cpp \
../type_info_tests.cpp \
../verification_tests.cpp 

OBJS += \
./demo_dests.o \
./gcc_stubbing_multiple_values_tests.o \
./gcc_type_info_tests.o \
./msc_stubbing_multiple_values_tests.o \
./msc_type_info_tests.o \
./referece_types_tests.o \
./sequence_verification_tests.o \
./stubbing_tests.o \
./tpunit++main.o \
./type_info_tests.o \
./verification_tests.o 

CPP_DEPS += \
./demo_dests.d \
./gcc_stubbing_multiple_values_tests.d \
./gcc_type_info_tests.d \
./msc_stubbing_multiple_values_tests.d \
./msc_type_info_tests.d \
./referece_types_tests.d \
./sequence_verification_tests.d \
./stubbing_tests.d \
./tpunit++main.d \
./type_info_tests.d \
./verification_tests.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I"../../include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -ftest-coverage -fprofile-arcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


