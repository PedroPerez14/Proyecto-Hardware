### P3 - Proyecto-Hardware
 Pedro José Pérez García (756642) y Fernando Peña Bes (756012)
# Pasos para flashear placa
> El proyecto de Eclipse con el que se trabaja se llama P3_PH. Si tuviera otro nombre, bastaría con cambiarlo en los comandos.

1. Compilar el proyecto y localizar el fichero `P3_PH.elf` en el directorio `Debug`.
2. Ejecutar el siguiente comando para generar un binario a partir del `.elf`:
	```
	arm-none-eabi-objcopy -O binary P3_PH.elf P3_PH.bin
	```
	> Para que el comando funcione tenemos que estar situados en el directorio `C:\Programas-Practicas-ISA\EclipseARM\sourcery-g++-lite-arm-2011.03\bin`, y además debe contener el fichero `P3_PH.elf`.
3. Concectar la placa al ordenador.
4. Ejecutar el siguiente comando para copiar el binario en la memoria flash de la placa.
	```
	openocd-0.7.0.exe -f test/arm-fdi-ucm.cfg -c "program P3_PH.bin 0x00000000"
	```
	> Para que el comando funcione tenemos que estar situados en el directorio `C:\Programas-Practicas-ISA\EclipseARM\openocd-0.7.0\bin`, y además debe contener el fichero `P3_PH.bin`.
5. Una vez hecho esto, si reiniciamos la placa, se debería cargar el juego.



