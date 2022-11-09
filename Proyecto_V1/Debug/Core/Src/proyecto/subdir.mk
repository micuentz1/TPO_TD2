################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/proyecto/MdE.c \
../Core/Src/proyecto/funciones.c 

OBJS += \
./Core/Src/proyecto/MdE.o \
./Core/Src/proyecto/funciones.o 

C_DEPS += \
./Core/Src/proyecto/MdE.d \
./Core/Src/proyecto/funciones.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/proyecto/%.o Core/Src/proyecto/%.su: ../Core/Src/proyecto/%.c Core/Src/proyecto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/TobíasGuerrero/Documents/TPO_TD2/TPO_TD2/Proyecto_V1/Core/Src/libraries" -I"C:/Users/TobíasGuerrero/Documents/TPO_TD2/TPO_TD2/Proyecto_V1/Core/Inc/libraries" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Users/TobíasGuerrero/Documents/TPO_TD2/TPO_TD2/Proyecto_V1/Core/Inc/proyecto" -I"C:/Users/TobíasGuerrero/Documents/TPO_TD2/TPO_TD2/Proyecto_V1/Core/Src/proyecto" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-proyecto

clean-Core-2f-Src-2f-proyecto:
	-$(RM) ./Core/Src/proyecto/MdE.d ./Core/Src/proyecto/MdE.o ./Core/Src/proyecto/MdE.su ./Core/Src/proyecto/funciones.d ./Core/Src/proyecto/funciones.o ./Core/Src/proyecto/funciones.su

.PHONY: clean-Core-2f-Src-2f-proyecto

