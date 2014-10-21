
RM := rm -rf

CPP_SRCS += \
custom_testing_framework_tests.cpp \
main.cpp  

OBJS += $(subst .cpp,.o,$(CPP_SRCS))

CPP_DEPS += $(subst .cpp,.d,$(CPP_SRCS))

all: fakeit_with_tpunit

check: fakeit_with_tpunit
	./fakeit_with_tpunit.exe

fakeit_with_tpunit: $(OBJS) 
	@echo 'Building test application: fakeit_with_tpunit.exe'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "fakeit_with_tpunit.exe" $(OBJS) 
	@echo 'Finished building test application: fakeit_with_tpunit.exe'
	@echo ' '

%.o: ../sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I"../../../include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifneq ($(MAKECMDGOALS),clean)
-include $(CPP_DEPS)
endif

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS) fakeit_with_tpunit.exe *.gc*
	-@echo ' '
