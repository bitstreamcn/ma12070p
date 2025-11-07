################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/libflac/src/libFLAC/bitmath.c \
../Core/Lib/libflac/src/libFLAC/bitreader.c \
../Core/Lib/libflac/src/libFLAC/bitwriter.c \
../Core/Lib/libflac/src/libFLAC/cpu.c \
../Core/Lib/libflac/src/libFLAC/crc.c \
../Core/Lib/libflac/src/libFLAC/fixed.c \
../Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.c \
../Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.c \
../Core/Lib/libflac/src/libFLAC/float.c \
../Core/Lib/libflac/src/libFLAC/format.c \
../Core/Lib/libflac/src/libFLAC/lpc.c \
../Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.c \
../Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.c \
../Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.c \
../Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.c \
../Core/Lib/libflac/src/libFLAC/md5.c \
../Core/Lib/libflac/src/libFLAC/memory.c \
../Core/Lib/libflac/src/libFLAC/metadata_iterators.c \
../Core/Lib/libflac/src/libFLAC/metadata_object.c \
../Core/Lib/libflac/src/libFLAC/stream_decoder.c \
../Core/Lib/libflac/src/libFLAC/stream_encoder_framing.c \
../Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.c \
../Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.c \
../Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.c \
../Core/Lib/libflac/src/libFLAC/window.c 

C_DEPS += \
./Core/Lib/libflac/src/libFLAC/bitmath.d \
./Core/Lib/libflac/src/libFLAC/bitreader.d \
./Core/Lib/libflac/src/libFLAC/bitwriter.d \
./Core/Lib/libflac/src/libFLAC/cpu.d \
./Core/Lib/libflac/src/libFLAC/crc.d \
./Core/Lib/libflac/src/libFLAC/fixed.d \
./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.d \
./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.d \
./Core/Lib/libflac/src/libFLAC/float.d \
./Core/Lib/libflac/src/libFLAC/format.d \
./Core/Lib/libflac/src/libFLAC/lpc.d \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.d \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.d \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.d \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.d \
./Core/Lib/libflac/src/libFLAC/md5.d \
./Core/Lib/libflac/src/libFLAC/memory.d \
./Core/Lib/libflac/src/libFLAC/metadata_iterators.d \
./Core/Lib/libflac/src/libFLAC/metadata_object.d \
./Core/Lib/libflac/src/libFLAC/stream_decoder.d \
./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.d \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.d \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.d \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.d \
./Core/Lib/libflac/src/libFLAC/window.d 

OBJS += \
./Core/Lib/libflac/src/libFLAC/bitmath.o \
./Core/Lib/libflac/src/libFLAC/bitreader.o \
./Core/Lib/libflac/src/libFLAC/bitwriter.o \
./Core/Lib/libflac/src/libFLAC/cpu.o \
./Core/Lib/libflac/src/libFLAC/crc.o \
./Core/Lib/libflac/src/libFLAC/fixed.o \
./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.o \
./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.o \
./Core/Lib/libflac/src/libFLAC/float.o \
./Core/Lib/libflac/src/libFLAC/format.o \
./Core/Lib/libflac/src/libFLAC/lpc.o \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.o \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.o \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.o \
./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.o \
./Core/Lib/libflac/src/libFLAC/md5.o \
./Core/Lib/libflac/src/libFLAC/memory.o \
./Core/Lib/libflac/src/libFLAC/metadata_iterators.o \
./Core/Lib/libflac/src/libFLAC/metadata_object.o \
./Core/Lib/libflac/src/libFLAC/stream_decoder.o \
./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.o \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.o \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.o \
./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.o \
./Core/Lib/libflac/src/libFLAC/window.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/libflac/src/libFLAC/%.o Core/Lib/libflac/src/libFLAC/%.su Core/Lib/libflac/src/libFLAC/%.cyclo: ../Core/Lib/libflac/src/libFLAC/%.c Core/Lib/libflac/src/libFLAC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -DSTM32_THREAD_SAFE_STRATEGY=4 -DUSE_MA12070P -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/Player/Inc" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Core/Lib/libflac/src/libFLAC/include" -I"J:/STM32CubeIDE/workspace_1.18.1/H743HiFi_MA12070P/Drivers/BSP/ma12070p/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-libflac-2f-src-2f-libFLAC

clean-Core-2f-Lib-2f-libflac-2f-src-2f-libFLAC:
	-$(RM) ./Core/Lib/libflac/src/libFLAC/bitmath.cyclo ./Core/Lib/libflac/src/libFLAC/bitmath.d ./Core/Lib/libflac/src/libFLAC/bitmath.o ./Core/Lib/libflac/src/libFLAC/bitmath.su ./Core/Lib/libflac/src/libFLAC/bitreader.cyclo ./Core/Lib/libflac/src/libFLAC/bitreader.d ./Core/Lib/libflac/src/libFLAC/bitreader.o ./Core/Lib/libflac/src/libFLAC/bitreader.su ./Core/Lib/libflac/src/libFLAC/bitwriter.cyclo ./Core/Lib/libflac/src/libFLAC/bitwriter.d ./Core/Lib/libflac/src/libFLAC/bitwriter.o ./Core/Lib/libflac/src/libFLAC/bitwriter.su ./Core/Lib/libflac/src/libFLAC/cpu.cyclo ./Core/Lib/libflac/src/libFLAC/cpu.d ./Core/Lib/libflac/src/libFLAC/cpu.o ./Core/Lib/libflac/src/libFLAC/cpu.su ./Core/Lib/libflac/src/libFLAC/crc.cyclo ./Core/Lib/libflac/src/libFLAC/crc.d ./Core/Lib/libflac/src/libFLAC/crc.o ./Core/Lib/libflac/src/libFLAC/crc.su ./Core/Lib/libflac/src/libFLAC/fixed.cyclo ./Core/Lib/libflac/src/libFLAC/fixed.d ./Core/Lib/libflac/src/libFLAC/fixed.o ./Core/Lib/libflac/src/libFLAC/fixed.su ./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.cyclo ./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.d ./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.o ./Core/Lib/libflac/src/libFLAC/fixed_intrin_sse2.su ./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.cyclo ./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.d ./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.o ./Core/Lib/libflac/src/libFLAC/fixed_intrin_ssse3.su ./Core/Lib/libflac/src/libFLAC/float.cyclo ./Core/Lib/libflac/src/libFLAC/float.d ./Core/Lib/libflac/src/libFLAC/float.o ./Core/Lib/libflac/src/libFLAC/float.su ./Core/Lib/libflac/src/libFLAC/format.cyclo ./Core/Lib/libflac/src/libFLAC/format.d ./Core/Lib/libflac/src/libFLAC/format.o ./Core/Lib/libflac/src/libFLAC/format.su ./Core/Lib/libflac/src/libFLAC/lpc.cyclo ./Core/Lib/libflac/src/libFLAC/lpc.d ./Core/Lib/libflac/src/libFLAC/lpc.o ./Core/Lib/libflac/src/libFLAC/lpc.su ./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.cyclo ./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.d ./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.o ./Core/Lib/libflac/src/libFLAC/lpc_intrin_avx2.su ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.cyclo ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.d ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.o ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse.su ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.cyclo ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.d ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.o ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse2.su ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.cyclo ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.d ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.o ./Core/Lib/libflac/src/libFLAC/lpc_intrin_sse41.su ./Core/Lib/libflac/src/libFLAC/md5.cyclo ./Core/Lib/libflac/src/libFLAC/md5.d ./Core/Lib/libflac/src/libFLAC/md5.o ./Core/Lib/libflac/src/libFLAC/md5.su ./Core/Lib/libflac/src/libFLAC/memory.cyclo ./Core/Lib/libflac/src/libFLAC/memory.d ./Core/Lib/libflac/src/libFLAC/memory.o ./Core/Lib/libflac/src/libFLAC/memory.su ./Core/Lib/libflac/src/libFLAC/metadata_iterators.cyclo ./Core/Lib/libflac/src/libFLAC/metadata_iterators.d ./Core/Lib/libflac/src/libFLAC/metadata_iterators.o ./Core/Lib/libflac/src/libFLAC/metadata_iterators.su ./Core/Lib/libflac/src/libFLAC/metadata_object.cyclo ./Core/Lib/libflac/src/libFLAC/metadata_object.d ./Core/Lib/libflac/src/libFLAC/metadata_object.o ./Core/Lib/libflac/src/libFLAC/metadata_object.su ./Core/Lib/libflac/src/libFLAC/stream_decoder.cyclo ./Core/Lib/libflac/src/libFLAC/stream_decoder.d ./Core/Lib/libflac/src/libFLAC/stream_decoder.o ./Core/Lib/libflac/src/libFLAC/stream_decoder.su ./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.cyclo ./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.d ./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.o ./Core/Lib/libflac/src/libFLAC/stream_encoder_framing.su ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.cyclo ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.d ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.o ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_avx2.su ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.cyclo ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.d ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.o ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_sse2.su ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.cyclo ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.d ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.o ./Core/Lib/libflac/src/libFLAC/stream_encoder_intrin_ssse3.su ./Core/Lib/libflac/src/libFLAC/window.cyclo ./Core/Lib/libflac/src/libFLAC/window.d ./Core/Lib/libflac/src/libFLAC/window.o ./Core/Lib/libflac/src/libFLAC/window.su

.PHONY: clean-Core-2f-Lib-2f-libflac-2f-src-2f-libFLAC

