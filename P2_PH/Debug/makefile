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
P2_PH.hex \

SECONDARY_LIST += \
P2_PH.lst \

SECONDARY_SIZE += \
P2_PH.siz \


# All Target
all: P2_PH.elf secondary-outputs

# Tool invocations
P2_PH.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"D:\workspace\P2_PH\common\ld_script.ld" -nostartfiles -L"C:\Programas-Practicas-ISA\EclipseARM\sourcery-g++-lite-arm-2011.03\bin" -Wl,-Map,P2_PH.map -mcpu=arm7tdmi -g3 -gdwarf-2 -o "P2_PH.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

P2_PH.hex: P2_PH.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary P2_PH.elf  "P2_PH.hex"
	@echo 'Finished building: $@'
	@echo ' '

P2_PH.lst: P2_PH.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S P2_PH.elf > "P2_PH.lst"
	@echo 'Finished building: $@'
	@echo ' '

P2_PH.siz: P2_PH.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley P2_PH.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) P2_PH.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
