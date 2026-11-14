#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

int main() {
    MisionArtemis4 mision;
    inicializarMision(&mision);
    
    int opcion;
    do {
        printf("\n==================================================\n");
        printf("   SIMULADOR ARTEMIS - SITUACION PROBLEMA N°4     \n");
        printf("   Reingreso Atmosferico, Frenado y Amerizaje     \n");
        printf("==================================================\n");
        printf("1. Ver parametros actuales de la mision\n");
        printf("2. Calculo realizado por la fuerza de rozamiento atmosferica\n");
        printf("3. Calculo de la desaceleracion media ejercida sobre la tripulacion \n");
        printf("4. Calculo fuerza promedio que el agua ejerce sobre la estructura \n");
        printf("5. Analisis angulo de reingreso y seguridad \n");
        printf("6. Modificar angulo de reingreso para pruebas\n");
        printf("7. Salir\n");
        
        opcion = leerEntero("Seleccione una opcion (1-7): ", 1, 7);
        
        switch (opcion) {
            case 1:
                printf("\n--- PARAMETROS ACTUALES DE LA MISION ---\n");
                printf(" - Masa: %.1f kg\n - Velocidad inicial: %.1f m/s\n - Altitud inicial: %.1f m\n", mision.masa, mision.v_inicial, mision.h_inicial);
                printf(" - Velocidad en paracaidas: %.1f m/s\n - Angulo actual: %.2f°\n", mision.v_paracaidas, mision.angulo_reingreso);
                break;
            case 2:
                calcularTrabajoRozamiento(mision);
                break;
            case 3:
                calcularDesaceleracionMedia(mision);
                break;
            case 4:
                calcularFuerzaAmerizaje(mision);
                break;
            case 5:
                analizarAnguloReingreso(mision);
                break;
            case 6:
                mision.angulo_reingreso = leerFlotante("Ingrese nuevo angulo de reingreso (ej. 4.5, 6.2, 8.0): ", 0.0f, 90.0f);
                analizarAnguloReingreso(mision);
                break;
            case 7:
                printf("\nSaliendo del simulador Artemis.\n");
                break;
            default:
                printf("\nOpcion no valida.\n");
        }
    } while (opcion != 7);
    
    return 0;
}