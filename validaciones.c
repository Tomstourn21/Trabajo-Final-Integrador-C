#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "validaciones.h"

#define LONG_LINEA 128

/*  */
/*  Funciones internas                                                 */
/*  */


static int leerLinea(char *buf, size_t n) {
    size_t len;
    int c;
    int hubo_resto = 0;

    if (fgets(buf, (int)n, stdin) == NULL) {
        printf("\n[ENTRADA CERRADA] No hay mas datos para leer. "
               "Finalizando el programa de forma segura.\n");
        exit(EXIT_FAILURE);
    }

    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
        if (len > 0 && buf[len - 1] == '\r') {
            buf[--len] = '\0';
        }
        return 1;
    }

   
    while ((c = getchar()) != '\n' && c != EOF) {
        hubo_resto = 1;
    }
    return hubo_resto ? 0 : 1;
}


static int soloEspacios(const char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}


/*  Interfaz                                                           */
/* ------------------------------------------------------------------ */
void mostrarSeparador(void) {
    printf("--------------------------------------------------------------\n");
}

void mostrarEncabezado(const char *titulo) {
    printf("\n==============================================================\n");
    printf("  %s\n", titulo);
    printf("==============================================================\n");
}

void mostrarError(const char *mensaje) {
    printf("  [ERROR] %s\n", mensaje);
}

void mostrarAdvertencia(const char *mensaje) {
    printf("  [ADVERTENCIA] %s\n", mensaje);
}

void esperarEnter(const char *mensaje) {
    char buf[LONG_LINEA];
    printf("%s", mensaje);
    leerLinea(buf, sizeof(buf));
}

/*  */
/*  Lectura segura                                                     */
/*  */

int leerEntero(const char *mensaje, int min, int max) {
    char buf[LONG_LINEA];
    char aviso[100];
    char *fin;
    long valor;

    for (;;) {
        printf("%s", mensaje);

        if (!leerLinea(buf, sizeof(buf))) {
            mostrarError("La entrada es demasiado larga.");
            continue;
        }
        if (soloEspacios(buf)) {
            mostrarError("No ingreso ningun dato.");
            continue;
        }

        errno = 0;
        valor = strtol(buf, &fin, 10);

        if (fin == buf || !soloEspacios(fin)) {
            mostrarError("Entrada invalida. Ingrese solo un numero entero.");
            continue;
        }
        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            mostrarError("El numero es demasiado grande.");
            continue;
        }
        if (valor < min || valor > max) {
            snprintf(aviso, sizeof(aviso),
                     "El valor debe estar entre %d y %d.", min, max);
            mostrarError(aviso);
            continue;
        }
        return (int)valor;
    }
}

double leerDecimal(const char *mensaje, double min, double max) {
    char buf[LONG_LINEA];
    char aviso[120];
    char *fin;
    char *p;
    double valor;

    for (;;) {
        printf("%s", mensaje);

        if (!leerLinea(buf, sizeof(buf))) {
            mostrarError("La entrada es demasiado larga.");
            continue;
        }
        if (soloEspacios(buf)) {
            mostrarError("No ingreso ningun dato.");
            continue;
        }

        if (strspn(buf, "0123456789+-.,eE \t") != strlen(buf)) {
            mostrarError("Entrada invalida. Ingrese solo un numero (ej. 6.2).");
            continue;
        }

        /* Se admite coma decimal (6,2) ademas de punto (6.2). */
        for (p = buf; *p != '\0'; p++) {
            if (*p == ',') {
                *p = '.';
            }
        }

        errno = 0;
        valor = strtod(buf, &fin);

        if (fin == buf || !soloEspacios(fin)) {
            mostrarError("Entrada invalida. Ingrese solo un numero (ej. 6.2).");
            continue;
        }
        if (errno == ERANGE || !isfinite(valor)) {
            mostrarError("El numero esta fuera del rango representable.");
            continue;
        }
        if (valor < min || valor > max) {
            snprintf(aviso, sizeof(aviso),
                     "El valor debe estar entre %.2f y %.2f.", min, max);
            mostrarError(aviso);
            continue;
        }
        return valor;
    }
}

char leerCaracter(const char *mensaje, const char *permitidos) {
    char buf[LONG_LINEA];
    char aviso[100];
    char *ini;
    char *fin;
    char c;

    for (;;) {
        printf("%s", mensaje);

        if (!leerLinea(buf, sizeof(buf))) {
            mostrarError("La entrada es demasiado larga.");
            continue;
        }

        ini = buf;
        while (isspace((unsigned char)*ini)) {
            ini++;
        }
        if (*ini == '\0') {
            mostrarError("No ingreso ningun dato.");
            continue;
        }

        fin = ini + 1;
        if (!soloEspacios(fin)) {
            mostrarError("Ingrese un unico caracter.");
            continue;
        }

        c = (char)tolower((unsigned char)*ini);
        if (strchr(permitidos, c) == NULL) {
            snprintf(aviso, sizeof(aviso),
                     "Opcion invalida. Opciones permitidas: %s", permitidos);
            mostrarError(aviso);
            continue;
        }
        return c;
    }
}
