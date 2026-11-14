#ifndef SIMULADOR_H
#define SIMULADOR_H

/* */
/*  Constantes                                                         */
/*  */
#define G_TERRESTRE            9.80
#define ANGULO_SEGURO_MIN      5.5
#define ANGULO_SEGURO_MAX      7.0 
#define V_IMPACTO_SEGURA       8.0
#define LIMITE_G_TRIPULACION   4.0 

#define N_PARAMETROS           9 
#define N_MUESTRAS_TELEMETRIA  9 
#define MAX_EVENTOS            4 
#define LONG_TEXTO             96

/*  */
/*  Tipos de datos                                                     */
/*  */

/* Estado completo de la mision (Situacion-Problema 4). */
typedef struct {
    double masa;
    double v_inicial;
    double h_inicial;
    double v_paracaidas;
    double h_paracaidas;
    double dt_desaceleracion;
    double v_impacto;
    double profundidad_agua;
    double angulo_reingreso;
} MisionArtemis4;

/* Descriptor de un parametro editable */
typedef struct {
    const char *nombre;
    const char *unidad;
    double     *valor;
    double      minimo;
    double      maximo;
} ParametroEditable;

/* Resultados de cada inciso (la logica NO imprime, solo calcula). */
typedef struct {
    double ki;
    double kf;
    double trabajo_grav;
    double trabajo_roz;
} ResultadoRozamiento;

typedef struct {
    double delta_v;
    double a_media;
    double g_multiplo;
} ResultadoDesaceleracion;

typedef struct {
    double peso;
    double energia_impacto;
    double fuerza_frenado;
    double f_prom;
    double multiplo_peso;
} ResultadoAmerizaje;

typedef enum {
    ANGULO_SEGURO,
    ANGULO_EMPINADO,
    ANGULO_TANGENCIAL 
} EstadoAngulo;

/* Evento registrado durante la simulacion por fases. */
typedef struct {
    char   fase[LONG_TEXTO];
    double tiempo;
    double altitud;
    double velocidad;
    int    superado; 
} EventoMision;

typedef struct {
    EventoMision eventos[MAX_EVENTOS];
    int cantidad;
    int puntaje; 
} ReporteMision;

/*  */
/*  Inicializacion y validacion del modelo                             */
/*  */
void inicializarMision(MisionArtemis4 *mision);
int  validarMision(const MisionArtemis4 *mision, const char **motivo);
int  obtenerParametros(MisionArtemis4 *mision, ParametroEditable tabla[], int max);

/*  */
/*  Calculos fisicos (sin entrada/salida)                              */
/*  */
void         calcularRozamiento(const MisionArtemis4 *m, ResultadoRozamiento *r);
void         calcularDesaceleracion(const MisionArtemis4 *m, ResultadoDesaceleracion *r);
void         calcularAmerizaje(const MisionArtemis4 *m, ResultadoAmerizaje *r);
EstadoAngulo clasificarAngulo(double angulo);
void         generarTelemetria(const MisionArtemis4 *m, double tiempos[],
                               double velocidades[], double disipada[], int n);

/*  */
/*  Presentacion de resultados y menus                                 */
/*  */
void mostrarParametros(MisionArtemis4 *mision);
void menuModificarParametros(MisionArtemis4 *mision);
void mostrarRozamiento(const MisionArtemis4 *mision);
void mostrarDesaceleracion(const MisionArtemis4 *mision);
void mostrarAmerizaje(const MisionArtemis4 *mision);
void mostrarAnguloReingreso(const MisionArtemis4 *mision);

/* Simulacion de la mision completa, fase por fase, con reporte final. */
void ejecutarSimulacion(const MisionArtemis4 *base);

#endif
