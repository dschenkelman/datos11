################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Block.cpp \
../src/BlockFile.cpp \
../src/CustomerComparer.cpp \
../src/ElectronicElections.cpp \
../src/Record.cpp 

OBJS += \
./src/Block.o \
./src/BlockFile.o \
./src/CustomerComparer.o \
./src/ElectronicElections.o \
./src/Record.o 

CPP_DEPS += \
./src/Block.d \
./src/BlockFile.d \
./src/CustomerComparer.d \
./src/ElectronicElections.d \
./src/Record.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


