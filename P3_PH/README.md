## P3 - Proyecto-Hardware
# Pasos para flashear placa

1. Compilar el proyecto y localizar el fichero `.elf` en el directorio `Debug`.
2. Ejecutar el siguiente comando para generar un binario a partir del `.elf`:
	```
	arm-none-eabi-objcopy -O binary prueba.elf prueba.bin
	```
	`C:\Programas-Practicas-ISA\EclipseARM\sourcery_c++\bin`
3. Concectar la placa al ordenador.
4. Ejecutar el siguiente comando para copiar el binario en la memoria flash de la placa.
	```
	openocd-0.7.0.exe -f test/arm-fdi-ucm.cfg -c "program prueba.bin 0x00000000"
	```
	`C:\Programas-Practicas-ISA\EclipseARM\openocd- 0.7.0\bin`



