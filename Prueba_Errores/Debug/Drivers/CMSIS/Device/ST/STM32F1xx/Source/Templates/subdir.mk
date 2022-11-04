################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.o Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.su: ../Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.c Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/ProyectosSTM32/Prueba_Errores/Core/Src/libraries" -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc/libraries" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc/proyecto" -I"D:/ProyectosSTM32/Prueba_Errores/Core/Src/proyecto" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/ProyectosSTM32/Prueba_Errores/Core/Inc" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/ProyectosSTM32/Prueba_Errores/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F1xx-2f-Source-2f-Templates

clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F1xx-2f-Source-2f-Templates:
	-$(RM) ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.d ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.o ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F1xx-2f-Source-2f-Templates

