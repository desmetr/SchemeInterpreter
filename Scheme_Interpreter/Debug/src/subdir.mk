################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Automaton.cpp \
../src/DFA.cpp \
../src/eNFA.cpp \
../src/main.cpp 

OBJS += \
./src/Automaton.o \
./src/DFA.o \
./src/eNFA.o \
./src/main.o 

CPP_DEPS += \
./src/Automaton.d \
./src/DFA.d \
./src/eNFA.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


