################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/libraries/OneWire.c \
../Core/Src/libraries/fatfs_sd.c \
../Core/Src/libraries/liquidcrystal_i2c.c \
../Core/Src/libraries/my_ftoa.c 

OBJS += \
./Core/Src/libraries/OneWire.o \
./Core/Src/libraries/fatfs_sd.o \
./Core/Src/libraries/liquidcrystal_i2c.o \
./Core/Src/libraries/my_ftoa.o 

C_DEPS += \
./Core/Src/libraries/OneWire.d \
./Core/Src/libraries/fatfs_sd.d \
./Core/Src/libraries/liquidcrystal_i2c.d \
./Core/Src/libraries/my_ftoa.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/libraries/%.o Core/Src/libraries/%.su: ../Core/Src/libraries/%.c Core/Src/libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/ProyectosSTM32/Prueba_Errores/Core/Src/libraries" -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc/libraries" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc/proyecto" -I"D:/ProyectosSTM32/Prueba_Errores/Core/Src/proyecto" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-libraries

clean-Core-2f-Src-2f-libraries:
	-$(RM) ./Core/Src/libraries/OneWire.d ./Core/Src/libraries/OneWire.o ./Core/Src/libraries/OneWire.su ./Core/Src/libraries/fatfs_sd.d ./Core/Src/libraries/fatfs_sd.o ./Core/Src/libraries/fatfs_sd.su ./Core/Src/libraries/liquidcrystal_i2c.d ./Core/Src/libraries/liquidcrystal_i2c.o ./Core/Src/libraries/liquidcrystal_i2c.su ./Core/Src/libraries/my_ftoa.d ./Core/Src/libraries/my_ftoa.o ./Core/Src/libraries/my_ftoa.su

.PHONY: clean-Core-2f-Src-2f-libraries

