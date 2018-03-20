LD := g++
CXX := g++

RM := rm -rf

-include sources.mk

OBJS += $(subst .cpp,.o,$(CPP_SRCS))

CPP_DEPS += $(subst .cpp,.d,$(CPP_SRCS))

EXE := .exe

all: fakeit_test_application

coverage: fakeit_test_application_with_coverage

check: fakeit_test_application
	./fakeit_tests$(EXE)

fakeit_test_application: $(OBJS)
	@echo 'Building test application: fakeit_tests$(EXE)'
	@echo 'Invoking: GCC C++ Linker'
	$(LD) -flto -o "fakeit_tests$(EXE)" $(OBJS)
	@echo 'Finished building test application: fakeit_tests$(EXE)'
	@echo ' '

fakeit_test_application_with_coverage: $(subst .cpp,_with_coverage,$(CPP_SRCS))
	@echo 'Building test application: fakeit_tests$(EXE)'
	@echo 'Invoking: GCC C++ Linker'
	$(LD) --coverage -o "fakeit_tests$(EXE)" $(OBJS)
	@echo 'Finished building test application: fakeit_tests$(EXE)'
	@echo ' '

%.o: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) -flto -D__GXX_EXPERIMENTAL_CXX0X__ -I"../include" -I"../config/standalone" -O0 -g3 -Wall -Wextra -Wno-ignored-qualifiers -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%_with_coverage: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) --coverage -D__GXX_EXPERIMENTAL_CXX0X__ -I"../include" -I"../config/standalone" -O0 -g3 -Wall -Wextra -Wno-ignored-qualifiers -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%_with_coverage=%.d)" -MT"$(@:%_with_coverage=%.d)" -o $(subst _with_coverage,.o,"$@") "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifneq ($(MAKECMDGOALS),clean)
-include $(CPP_DEPS)
endif

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS) fakeit_tests$(EXE) *.gc*
	-@echo ' '
