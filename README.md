# Trabajo-Final-Integrador-C
-Integrantes:

Tourn, Tomás Joaquín - Legajo 31388

Perez, Nahuel Lautaro - Legajo 31448

Hernández Suárez, Augusto Iván - Legajo 31569


-Escenario:

Tras completar el sobrevuelo lunar, el módulo de tripulación Orion (m = 10.400,0 kg) se separa del módulo de servicio y reingresa a la atmósfera terrestre a una velocidad hiperveloz de vi = 11,0 km/s (11.000,0 m/s, equivalente a 39.600,0 km/h o Mach 32) a una altitud de h = 120,0 km (120.000,0 m). El escudo térmico disipa la energía cinética mediante el trabajo de la fuerza de resistencia aerodinámica del aire (Froz), reduciendo la rapidez hasta vf = 25,0 m/s al momento de desplegar los paracaídas principales a una altitud de h = 2,50 km (2.500,0 m). Finalmente, la nave impacta en la superficie del Océano Pacífico (amerizaje) a una rapidez final segura de vimpacto = 8,0 m/s.

Ejemplos de preguntas de análisis y cálculo analítico que el programa debería responder, a partir de distintos valores de entrada ingresados por el usuario:

Calcule el trabajo total Wroz realizado por la fuerza de rozamiento atmosférico sobre la cápsula desde el reingreso (120.000,0 m) hasta la apertura de los paracaídas (2.500,0 m).

Si la fase de mayor desaceleración dura Δt = 400,0 s, determine la desaceleración media ejercida sobre la tripulación en m/s² y en múltiplos de la gravedad (g).

Suponiendo que durante el amerizaje la cápsula se hunde d = 1,50 m en el agua hasta detenerse momentáneamente antes de flotar, determine la fuerza promedio Fprom que el agua ejerce sobre la estructura. Exprese el resultado en Newtons (N) y como múltiplo del peso de la nave.

Analice qué sucedería si el ángulo de reingreso γ estuviera fuera de la ventana de seguridad (5,5° ≤ γ ≤ 7,0°): evalúe las consecuencias físicas si γ > 7,0° (desaceleración excesiva) o si γ < 5,5° (rebote atmosférico)


-Funcionamiento:

Tras compilar los archivos se genera un ejecutable .exe, el cual nuestra un menu con 9 opciones:

" Ver parámetros actuales de la misión": Muestra los datos de la ultima modificación de parámetros.

"Modificar parámetros de la misión": Modifica los parámetros de la misión, modifica el resultado de las siguientes funciones.

"Restablecer valores por defecto": Si se modificaron los valores de la misión los reestablece a los que estaban por defecto, los que se dan en el enunciado.

"Trabajo de la fuerza de rozamiento atmosférico": Ejecuta los cálculos físicos para hallar el rozamiento atmosférico.

"Desaceleración media sobre la tripulación" : Determina la desaceleración media ejercida sobre la tripulación.

"Fuerza promedio del agua en el amerizaje":  Calcula la fuerza promedio que el agua ejerce sobre la capsula durante el amerizaje.

"Análisis del Angulo de reingreso": Informa si el ángulo ingresado en los parámetros esta dentro de la ventana de seguridad y evalúa las consecuencias  físicas de esto.

"SIMULAR MISION COMPLETA ": Simula como seria el amerizaje en base a los parámetros ingresados y evalúa si la misión fue exitosa.

" Salir": Cierra el programa. 

-Instrucciones de compilación y ejecución:

Compilar:
Windows: .\compilar.bat
Manualmente: gcc -Wall -Wextra -std=c11 main.c simulador.c validaciones.c -o simulador_artemis.exe -lm

Ejecutar:
.\simulador_artemis.exe

-Registro del uso de Inteligencia Artificial:

Consultas  sobre calculos y formulas fisicas.

