################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../work_in_progress/Environment.cpp \
../work_in_progress/Expression.cpp \
../work_in_progress/Lambda.cpp \
../work_in_progress/Parser.cpp \
../work_in_progress/main.cpp \
../work_in_progress/trash.cpp 

OBJS += \
./work_in_progress/Environment.o \
./work_in_progress/Expression.o \
./work_in_progress/Lambda.o \
./work_in_progress/Parser.o \
./work_in_progress/main.o \
./work_in_progress/trash.o 

CPP_DEPS += \
./work_in_progress/Environment.d \
./work_in_progress/Expression.d \
./work_in_progress/Lambda.d \
./work_in_progress/Parser.d \
./work_in_progress/main.d \
./work_in_progress/trash.d 


# Each subdirectory must supply rules for building sources it contributes
work_in_progress/%.o: ../work_in_progress/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


