################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/Player/Src/controller.c \
../Core/Lib/Player/Src/files.c \
../Core/Lib/Player/Src/flac_decoder.c \
../Core/Lib/Player/Src/flac_reader.c \
../Core/Lib/Player/Src/logger.c \
../Core/Lib/Player/Src/player.c \
../Core/Lib/Player/Src/ring_buffer.c \
../Core/Lib/Player/Src/utils.c 

C_DEPS += \
./Core/Lib/Player/Src/controller.d \
./Core/Lib/Player/Src/files.d \
./Core/Lib/Player/Src/flac_decoder.d \
./Core/Lib/Player/Src/flac_reader.d \
./Core/Lib/Player/Src/logger.d \
./Core/Lib/Player/Src/player.d \
./Core/Lib/Player/Src/ring_buffer.d \
./Core/Lib/Player/Src/utils.d 

OBJS += \
./Core/Lib/Player/Src/controller.o \
./Core/Lib/Player/Src/files.o \
./Core/Lib/Player/Src/flac_decoder.o \
./Core/Lib/Player/Src/flac_reader.o \
./Core/Lib/Player/Src/logger.o \
./Core/Lib/Player/Src/player.o \
./Core/Lib/Player/Src/ring_buffer.o \
./Core/Lib/Player/Src/utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/Player/Src/%.o Core/Lib/Player/Src/%.su Core/Lib/Player/Src/%.cyclo: ../Core/Lib/Player/Src/%.c Core/Lib/Player/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -DSTM32_THREAD_SAFE_STRATEGY=4 -DUSE_MA12070P -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/Player/Inc" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/src/libFLAC/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Drivers/BSP/ma12070p/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-Player-2f-Src

clean-Core-2f-Lib-2f-Player-2f-Src:
	-$(RM) ./Core/Lib/Player/Src/controller.cyclo ./Core/Lib/Player/Src/controller.d ./Core/Lib/Player/Src/controller.o ./Core/Lib/Player/Src/controller.su ./Core/Lib/Player/Src/files.cyclo ./Core/Lib/Player/Src/files.d ./Core/Lib/Player/Src/files.o ./Core/Lib/Player/Src/files.su ./Core/Lib/Player/Src/flac_decoder.cyclo ./Core/Lib/Player/Src/flac_decoder.d ./Core/Lib/Player/Src/flac_decoder.o ./Core/Lib/Player/Src/flac_decoder.su ./Core/Lib/Player/Src/flac_reader.cyclo ./Core/Lib/Player/Src/flac_reader.d ./Core/Lib/Player/Src/flac_reader.o ./Core/Lib/Player/Src/flac_reader.su ./Core/Lib/Player/Src/logger.cyclo ./Core/Lib/Player/Src/logger.d ./Core/Lib/Player/Src/logger.o ./Core/Lib/Player/Src/logger.su ./Core/Lib/Player/Src/player.cyclo ./Core/Lib/Player/Src/player.d ./Core/Lib/Player/Src/player.o ./Core/Lib/Player/Src/player.su ./Core/Lib/Player/Src/ring_buffer.cyclo ./Core/Lib/Player/Src/ring_buffer.d ./Core/Lib/Player/Src/ring_buffer.o ./Core/Lib/Player/Src/ring_buffer.su ./Core/Lib/Player/Src/utils.cyclo ./Core/Lib/Player/Src/utils.d ./Core/Lib/Player/Src/utils.o ./Core/Lib/Player/Src/utils.su

.PHONY: clean-Core-2f-Lib-2f-Player-2f-Src

