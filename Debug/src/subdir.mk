################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChessGame.cpp \
../src/board_move.cpp \
../src/eval.cpp \
../src/eval_consts.cpp \
../src/main.cpp \
../src/move_get_sort.cpp \
../src/pawn_eval.cpp \
../src/quiessence.cpp \
../src/search_base.cpp \
../src/trans_table.cpp 

OBJS += \
./src/ChessGame.o \
./src/board_move.o \
./src/eval.o \
./src/eval_consts.o \
./src/main.o \
./src/move_get_sort.o \
./src/pawn_eval.o \
./src/quiessence.o \
./src/search_base.o \
./src/trans_table.o 

CPP_DEPS += \
./src/ChessGame.d \
./src/board_move.d \
./src/eval.d \
./src/eval_consts.d \
./src/main.d \
./src/move_get_sort.d \
./src/pawn_eval.d \
./src/quiessence.d \
./src/search_base.d \
./src/trans_table.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


