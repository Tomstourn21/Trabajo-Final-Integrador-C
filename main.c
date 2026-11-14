#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

int main(void) {
    MisionArtemis4 mision;
    int opcion;

    inicializarMision(&mision);

    do {
        printf("\n  \n");
        printf("   SIMULADOR ARTEMIS - SITUACION PROBLEMA N.4\n");
        printf("   Reingreso Atmosferico, Frenado y Amerizaje\n");
        printf("    \n");
        printf(" 1. Ver parametros actuales de la mision\n");
        printf(" 2. Modificar parametros de la mision\n");
        printf(" 3. Restablecer valores por defecto\n");
        printf(" 4. a) Trabajo de la fuerza de rozamiento atmosferico\n");
        printf(" 5. b) Desaceleracion media sobre la tripulacion\n");
        printf(" 6. c) Fuerza promedio del agua en el amerizaje\n");
        printf(" 7. d) Analisis del angulo de reingreso\n");
        printf(" 8. SIMULAR MISION COMPLETA (por fases + reporte final)\n");
        printf(" 9. Salir\n");

        opcion = leerEntero("Seleccione una opcion (1-9): ", 1, 9);

        switch (opcion) {
            case 1: mostrarParametros(&mision);          break;
            case 2: menuModificarParametros(&mision);    break;
            case 3:
                inicializarMision(&mision);
                printf("\n  Parametros restablecidos a los valores del enunciado.\n");
                break;
            case 4: mostrarRozamiento(&mision);          break;
            case 5: mostrarDesaceleracion(&mision);      break;
            case 6: mostrarAmerizaje(&mision);           break;
            case 7: mostrarAnguloReingreso(&mision);     break;
            case 8: ejecutarSimulacion(&mision);         break;
            case 9: printf("\nSaliendo del simulador Artemis.\n"); break;
        }
    } while (opcion != 9);

    return 0;
}

