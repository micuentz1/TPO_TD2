################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/libraries/OneWire.c \
../Core/Src/libraries/liquidcrystal_i2c.c \
../Core/Src/libraries/my_ftoa.c 

OBJS += \
./Core/Src/libraries/OneWire.o \
./Core/Src/libraries/liquidcrystal_i2c.o \
./Core/Src/libraries/my_ftoa.o 

C_DEPS += \
./Core/Src/libraries/OneWire.d \
./Core/Src/libraries/liquidcrystal_i2c.d \
./Core/Src/libraries/my_ftoa.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/libraries/%.o Core/Src/libraries/%.su: ../Core/Src/libraries/%.c Core/Src/libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/jron0/Desktop/UTN/Tecnicas Digitales 2/TPO/Tests/Proyecto_V1/Core/Src/libraries" -I"C:/Users/jron0/Desktop/UTN/Tecnicas Digitales 2/TPO/Tests/Proyecto_V1/Core/Inc/libraries" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-libraries

clean-Core-2f-Src-2f-libraries:
	-$(RM) ./Core/Src/libraries/OneWire.d ./Core/Src/libraries/OneWire.o ./Core/Src/libraries/OneWire.su ./Core/Src/libraries/liquidcrystal_i2c.d ./Core/Src/libraries/liquidcrystal_i2c.o ./Core/Src/libraries/liquidcrystal_i2c.su ./Core/Src/libraries/my_ftoa.d ./Core/Src/libraries/my_ftoa.o ./Core/Src/libraries/my_ftoa.su

.PHONY: clean-Core-2f-Src-2f-libraries

