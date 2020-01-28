################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D"PRINTF_FLOAT_ENABLE" -D"SCANF_FLOAT_ENABLE" -I"C:/Users/Johannes/Desktop/Uni/1_Autonomes_Auto/Model-M/Model_M_Motor_Control/Static_Code/PDD" -I"C:/Users/Johannes/Desktop/Uni/1_Autonomes_Auto/Model-M/Model_M_Motor_Control/Static_Code/IO_Map" -I"C:/Users/Johannes/Desktop/Uni/1_Autonomes_Auto/Model-M/Model_M_Motor_Control/Sources" -I"C:/Users/Johannes/Desktop/Uni/1_Autonomes_Auto/Model-M/Model_M_Motor_Control/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


