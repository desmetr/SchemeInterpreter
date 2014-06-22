################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/Automaton.o \
../src/DFA.o \
../src/Environment.o \
../src/Evaluate.o \
../src/Expression.o \
../src/Lambda.o \
../src/Parser.o \
../src/eNFA.o \
../src/ini_configuration.o \
../src/main.o \
../src/readInput.o \
../src/trash.o 

CPP_SRCS += \
../src/Automaton.cpp \
../src/DFA.cpp \
../src/Environment.cpp \
../src/Evaluate.cpp \
../src/Expression.cpp \
../src/Lambda.cpp \
../src/Parser.cpp \
../src/eNFA.cpp \
../src/ini_configuration.cpp \
../src/main.cpp \
../src/readInput.cpp \
../src/trash.cpp 

OBJS += \
./src/Automaton.o \
./src/DFA.o \
./src/Environment.o \
./src/Evaluate.o \
./src/Expression.o \
./src/Lambda.o \
./src/Parser.o \
./src/eNFA.o \
./src/ini_configuration.o \
./src/main.o \
./src/readInput.o \
./src/trash.o 

CPP_DEPS += \
./src/Automaton.d \
./src/DFA.d \
./src/Environment.d \
./src/Evaluate.d \
./src/Expression.d \
./src/Lambda.d \
./src/Parser.d \
./src/eNFA.d \
./src/ini_configuration.d \
./src/main.d \
./src/readInput.d \
./src/trash.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


