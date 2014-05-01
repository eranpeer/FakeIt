
RM := rm -rf

CPP_SRCS += \
default_behaviore_tests.cpp \
gcc_stubbing_multiple_values_tests.cpp \
gcc_type_info_tests.cpp \
msc_stubbing_multiple_values_tests.cpp \
msc_type_info_tests.cpp \
referece_types_tests.cpp \
sequence_verification_tests.cpp \
stubbing_tests.cpp \
tpunit++main.cpp \
type_info_tests.cpp \
verification_tests.cpp \
remove_const_volatile_tests.cpp \
verification_error_formatting_tests.cpp 

OBJS += $(subst .cpp,.o,$(CPP_SRCS))

CPP_DEPS += $(subst .cpp,.d,$(CPP_SRCS))

all: fakeit_test_application

coverage: fakeit_test_application_with_coverage

fakeit_test_application: $(OBJS) 
	@echo 'Building test application: fakeit_tests.exe'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "fakeit_tests.exe" $(OBJS) 
	@echo 'Finished building test application: fakeit_tests.exe'
	@echo ' '

fakeit_test_application_with_coverage: $(subst .cpp,_with_coverage,$(CPP_SRCS)) 
	@echo 'Building test application: fakeit_tests.exe'
	@echo 'Invoking: GCC C++ Linker'
	g++ --coverage -o "fakeit_tests.exe" $(OBJS) 
	@echo 'Finished building test application: fakeit_tests.exe'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I"../../include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%_with_coverage: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --coverage -D__GXX_EXPERIMENTAL_CXX0X__ -I"../../include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%_with_coverage=%.d)" -MT"$(@:%_with_coverage=%.d)" -o $(subst _with_coverage,.o,"$@") "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifneq ($(MAKECMDGOALS),clean)
-include $(CPP_DEPS)
endif

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS) fakeit_tests.exe *.gc*
	-@echo ' '
