################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TestPhase1/TestPhase1.cpp \
../TestPhase1/ini_configuration.cpp \
../TestPhase1/readInput.cpp 

OBJS += \
./TestPhase1/TestPhase1.o \
./TestPhase1/ini_configuration.o \
./TestPhase1/readInput.o 

CPP_DEPS += \
./TestPhase1/TestPhase1.d \
./TestPhase1/ini_configuration.d \
./TestPhase1/readInput.d 


# Each subdirectory must supply rules for building sources it contributes
TestPhase1/%.o: ../TestPhase1/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


