@echo off
gcc -Wall -Wextra -std=c11 main.c simulador.c validaciones.c -o simulador_artemis.exe -lm
if %errorlevel%==0 (echo Compilado OK. Ejecutar: simulador_artemis.exe) else (echo Error de compilacion)
pause
