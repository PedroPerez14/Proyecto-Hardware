################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
P1_PH_v2.hex \

SECONDARY_LIST += \
P1_PH_v2.lst \

SECONDARY_SIZE += \
P1_PH_v2.siz \


# All Target
all: P1_PH_v2.elf secondary-outputs

# Tool invocations
P1_PH_v2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"C:\hlocal\workspace\P1_PH_v2\common\ld_script.ld" -nostartfiles -L"C:\Programas-Practicas-ISA\EclipseARM\sourcery-g++-lite-arm-2011.03\bin" -Wl,-Map,P1_PH_v2.map -mcpu=arm7tdmi -g3 -gdwarf-2 -o "P1_PH_v2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

P1_PH_v2.hex: P1_PH_v2.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary P1_PH_v2.elf  "P1_PH_v2.hex"
	@echo 'Finished building: $@'
	@echo ' '

P1_PH_v2.lst: P1_PH_v2.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S P1_PH_v2.elf > "P1_PH_v2.lst"
	@echo 'Finished building: $@'
	@echo ' '

P1_PH_v2.siz: P1_PH_v2.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley P1_PH_v2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) P1_PH_v2.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
