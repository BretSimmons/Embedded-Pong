################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ADC.c \
../Src/TIM.c \
../Src/button.c \
../Src/game.c \
../Src/main.c \
../Src/segment.c \
../Src/ssd1306.c \
../Src/ssd1306_fonts.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/ADC.o \
./Src/TIM.o \
./Src/button.o \
./Src/game.o \
./Src/main.o \
./Src/segment.o \
./Src/ssd1306.o \
./Src/ssd1306_fonts.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/ADC.d \
./Src/TIM.d \
./Src/button.d \
./Src/game.d \
./Src/main.d \
./Src/segment.d \
./Src/ssd1306.d \
./Src/ssd1306_fonts.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ADC.cyclo ./Src/ADC.d ./Src/ADC.o ./Src/ADC.su ./Src/TIM.cyclo ./Src/TIM.d ./Src/TIM.o ./Src/TIM.su ./Src/button.cyclo ./Src/button.d ./Src/button.o ./Src/button.su ./Src/game.cyclo ./Src/game.d ./Src/game.o ./Src/game.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/segment.cyclo ./Src/segment.d ./Src/segment.o ./Src/segment.su ./Src/ssd1306.cyclo ./Src/ssd1306.d ./Src/ssd1306.o ./Src/ssd1306.su ./Src/ssd1306_fonts.cyclo ./Src/ssd1306_fonts.d ./Src/ssd1306_fonts.o ./Src/ssd1306_fonts.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32l4xx.cyclo ./Src/system_stm32l4xx.d ./Src/system_stm32l4xx.o ./Src/system_stm32l4xx.su

.PHONY: clean-Src

