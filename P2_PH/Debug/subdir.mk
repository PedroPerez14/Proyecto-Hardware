################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../botones_antirebotes.c \
../button.c \
../cola_depuracion.c \
../jugada_por_botones.c \
../led.c \
../main.c \
../main_medidas_optimizaciones.c \
../pruebas_patron_volteo.c \
../pruebas_timer2.c \
../reversi8_2019.c \
../reversi_main.c \
../rutinas_excepciones.c \
../timer.c \
../timer2.c 

ASM_SRCS += \
../patron_volteo_arm_arm.asm \
../patron_volteo_arm_arm_opt.asm \
../patron_volteo_arm_c.asm 

OBJS += \
./8led.o \
./botones_antirebotes.o \
./button.o \
./cola_depuracion.o \
./jugada_por_botones.o \
./led.o \
./main.o \
./main_medidas_optimizaciones.o \
./patron_volteo_arm_arm.o \
./patron_volteo_arm_arm_opt.o \
./patron_volteo_arm_c.o \
./pruebas_patron_volteo.o \
./pruebas_timer2.o \
./reversi8_2019.o \
./reversi_main.o \
./rutinas_excepciones.o \
./timer.o \
./timer2.o 

C_DEPS += \
./8led.d \
./botones_antirebotes.d \
./button.d \
./cola_depuracion.d \
./jugada_por_botones.d \
./led.d \
./main.d \
./main_medidas_optimizaciones.d \
./pruebas_patron_volteo.d \
./pruebas_timer2.d \
./reversi8_2019.d \
./reversi_main.d \
./rutinas_excepciones.d \
./timer.d \
./timer2.d 

ASM_DEPS += \
./patron_volteo_arm_arm.d \
./patron_volteo_arm_arm_opt.d \
./patron_volteo_arm_c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"D:\workspace\P2_PH\common" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"D:\workspace\P2_PH\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


