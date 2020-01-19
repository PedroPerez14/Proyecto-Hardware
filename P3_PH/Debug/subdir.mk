################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../Bmp.c \
../botones_antirebotes.c \
../button.c \
../cola_depuracion.c \
../elementos_pantalla.c \
../funciones_itoa.c \
../jugada_por_botones.c \
../keyboard.c \
../lcd.c \
../led.c \
../main.c \
../main_medidas_optimizaciones.c \
../pruebas_patron_volteo.c \
../pruebas_timer2.c \
../reversi8_2019.c \
../reversi_main.c \
../rutinas_excepciones.c \
../teclado_antirebotes.c \
../timer.c \
../timer2.c \
../tp.c \
../tsp_antirebotes.c 

ASM_SRCS += \
../patron_volteo_arm_arm.asm \
../patron_volteo_arm_arm_opt.asm \
../patron_volteo_arm_c.asm 

OBJS += \
./8led.o \
./Bmp.o \
./botones_antirebotes.o \
./button.o \
./cola_depuracion.o \
./elementos_pantalla.o \
./funciones_itoa.o \
./jugada_por_botones.o \
./keyboard.o \
./lcd.o \
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
./teclado_antirebotes.o \
./timer.o \
./timer2.o \
./tp.o \
./tsp_antirebotes.o 

C_DEPS += \
./8led.d \
./Bmp.d \
./botones_antirebotes.d \
./button.d \
./cola_depuracion.d \
./elementos_pantalla.d \
./funciones_itoa.d \
./jugada_por_botones.d \
./keyboard.d \
./lcd.d \
./led.d \
./main.d \
./main_medidas_optimizaciones.d \
./pruebas_patron_volteo.d \
./pruebas_timer2.d \
./reversi8_2019.d \
./reversi_main.d \
./rutinas_excepciones.d \
./teclado_antirebotes.d \
./timer.d \
./timer2.d \
./tp.d \
./tsp_antirebotes.d 

ASM_DEPS += \
./patron_volteo_arm_arm.d \
./patron_volteo_arm_arm_opt.d \
./patron_volteo_arm_c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\hlocal\workspace\P3_PH\common" -O3 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\hlocal\workspace\P3_PH\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


