################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/Voting/subdir.mk
-include src/Vigenere/subdir.mk
-include src/VariableBlocks/subdir.mk
-include src/RSA/subdir.mk
-include src/Indexes/subdir.mk
-include src/Helpers/subdir.mk
-include src/Hash/subdir.mk
-include src/EntitiesTests/subdir.mk
-include src/Entities/subdir.mk
-include src/subdir.mk
-include src/BlocksTests/subdir.mk
-include src/BPlusTreeTests/subdir.mk
-include src/BPlusTree/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ElectronicElections

# Tool invocations
ElectronicElections: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"ElectronicElections" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ElectronicElections
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
