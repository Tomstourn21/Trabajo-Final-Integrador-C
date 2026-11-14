#ifndef VALIDACIONES_H
#define VALIDACIONES_H

void mostrarEncabezado(const char *titulo);
void mostrarSeparador(void);
void mostrarError(const char *mensaje);
void mostrarAdvertencia(const char *mensaje);
void esperarEnter(const char *mensaje);


int    leerEntero(const char *mensaje, int min, int max);

double leerDecimal(const char *mensaje, double min, double max);

char   leerCaracter(const char *mensaje, const char *permitidos);

#endif
