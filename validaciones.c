#include <stdio.h>
#include "validaciones.h"

int leerEntero(const char *mensaje, int min, int max) {
    int valor;
    char c;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &valor) == 1) {
            if ((c = getchar()) == '\n' || c == '\r') {
                if (valor >= min && valor <= max) {
                    return valor;
                } else {
                    printf("Error: El valor debe estar entre %d y %d.\n", min, max);
                }
            } else {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Error: Entrada invalida. Ingrese un numero entero.\n");
            }
        } else {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Error: Entrada invalida. Ingrese un numero entero.\n");
        }
    }
}

float leerFlotante(const char *mensaje, float min, float max) {
    float valor;
    char c;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &valor) == 1) {
            if ((c = getchar()) == '\n' || c == '\r') {
                if (valor >= min && valor <= max) {
                    return valor;
                } else {
                    printf("Error: El valor debe estar entre %.2f y %.2f.\n", min, max);
                }
            } else {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Error: Entrada invalida. Ingrese un numero flotante.\n");
            }
        } else {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Error: Entrada invalida. Ingrese un numero flotante.\n");
        }
    }
}
