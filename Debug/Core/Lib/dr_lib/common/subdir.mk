################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/dr_lib/common/dr_common.c 

C_DEPS += \
./Core/Lib/dr_lib/common/dr_common.d 

OBJS += \
./Core/Lib/dr_lib/common/dr_common.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/dr_lib/common/%.o Core/Lib/dr_lib/common/%.su Core/Lib/dr_lib/common/%.cyclo: ../Core/Lib/dr_lib/common/%.c Core/Lib/dr_lib/common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -DSTM32_THREAD_SAFE_STRATEGY=4 -DUSE_MA12070P -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/Player/Inc" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/src/libFLAC/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Drivers/BSP/ma12070p/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-dr_lib-2f-common

clean-Core-2f-Lib-2f-dr_lib-2f-common:
	-$(RM) ./Core/Lib/dr_lib/common/dr_common.cyclo ./Core/Lib/dr_lib/common/dr_common.d ./Core/Lib/dr_lib/common/dr_common.o ./Core/Lib/dr_lib/common/dr_common.su

.PHONY: clean-Core-2f-Lib-2f-dr_lib-2f-common

