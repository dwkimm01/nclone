################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NCApp.cpp \
../src/NCClientIf.cpp \
../src/NCClientPurple.cpp \
../src/NCCmdHistory.cpp \
../src/NCColor.cpp \
../src/NCEnvironmentVariables.cpp \
../src/NCException.cpp \
../src/NCExceptionCurses.cpp \
../src/NCExceptionOutOfRange.cpp \
../src/NCMain.cpp \
../src/NCObject.cpp \
../src/NCPathUtils.cpp \
../src/NCString.cpp \
../src/NCTimeUtils.cpp \
../src/NCWin.cpp \
../src/NCWinBuffer.cpp \
../src/NCWinScrollback.cpp \
../src/NCWinTime.cpp 

OBJS += \
./src/NCApp.o \
./src/NCClientIf.o \
./src/NCClientPurple.o \
./src/NCCmdHistory.o \
./src/NCColor.o \
./src/NCEnvironmentVariables.o \
./src/NCException.o \
./src/NCExceptionCurses.o \
./src/NCExceptionOutOfRange.o \
./src/NCMain.o \
./src/NCObject.o \
./src/NCPathUtils.o \
./src/NCString.o \
./src/NCTimeUtils.o \
./src/NCWin.o \
./src/NCWinBuffer.o \
./src/NCWinScrollback.o \
./src/NCWinTime.o 

CPP_DEPS += \
./src/NCApp.d \
./src/NCClientIf.d \
./src/NCClientPurple.d \
./src/NCCmdHistory.d \
./src/NCColor.d \
./src/NCEnvironmentVariables.d \
./src/NCException.d \
./src/NCExceptionCurses.d \
./src/NCExceptionOutOfRange.d \
./src/NCMain.d \
./src/NCObject.d \
./src/NCPathUtils.d \
./src/NCString.d \
./src/NCTimeUtils.d \
./src/NCWin.d \
./src/NCWinBuffer.d \
./src/NCWinScrollback.d \
./src/NCWinTime.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/include/libpurple -I/home/dwkimm01/apps/include -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/glib-2.0 -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


