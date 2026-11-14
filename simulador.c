#include <stdio.h>
#include <string.h>
#include "simulador.h"
#include "validaciones.h"

/*  */
/*  Inicializacion y validacion                                        */
/*  */

/* Valores por defecto: los del enunciado (Situacion-Problema 4). */
void inicializarMision(MisionArtemis4 *mision) {
    mision->masa              = 10400.0;
    mision->v_inicial         = 11000.0;
    mision->h_inicial         = 120000.0;
    mision->v_paracaidas      = 25.0;
    mision->h_paracaidas      = 2500.0;
    mision->dt_desaceleracion = 400.0;
    mision->v_impacto         = 8.0;
    mision->profundidad_agua  = 1.50;
    mision->angulo_reingreso  = 6.20;
}

int validarMision(const MisionArtemis4 *mision, const char **motivo) {
    if (mision->h_paracaidas >= mision->h_inicial) {
        *motivo = "La altitud de paracaidas debe ser menor que la altitud de reingreso.";
        return 0;
    }
    if (mision->v_paracaidas >= mision->v_inicial) {
        *motivo = "La rapidez al abrir paracaidas debe ser menor que la rapidez de reingreso.";
        return 0;
    }
    if (mision->v_impacto >= mision->v_paracaidas) {
        *motivo = "La rapidez de impacto debe ser menor que la rapidez al abrir paracaidas.";
        return 0;
    }
    *motivo = "";
    return 1;
}


int obtenerParametros(MisionArtemis4 *mision, ParametroEditable tabla[], int max) {
    const ParametroEditable base[N_PARAMETROS] = {
        { "Masa del modulo",              "kg",     &mision->masa,              100.0, 100000.0 },
        { "Rapidez de reingreso",         "m/s",    &mision->v_inicial,         100.0,  15000.0 },
        { "Altitud de reingreso",         "m",      &mision->h_inicial,       10000.0, 200000.0 },
        { "Rapidez al abrir paracaidas",  "m/s",    &mision->v_paracaidas,        1.0,   1000.0 },
        { "Altitud de paracaidas",        "m",      &mision->h_paracaidas,      100.0,  50000.0 },
        { "Duracion del frenado maximo",  "s",      &mision->dt_desaceleracion,   1.0,   3600.0 },
        { "Rapidez de impacto (amerizaje)", "m/s",  &mision->v_impacto,           0.1,    100.0 },
        { "Profundidad de frenado en agua", "m",    &mision->profundidad_agua,    0.1,     50.0 },
        { "Angulo de reingreso (gamma)",  "grados", &mision->angulo_reingreso,    0.1,     90.0 }
    };
    int i;
    int n = (max < N_PARAMETROS) ? max : N_PARAMETROS;

    for (i = 0; i < n; i++) {
        tabla[i] = base[i];
    }
    return n;
}

/*  */
/*  Calculos fisicos                                                   */
/*  */

/* Teorema trabajo-energia: W_total = W_grav + W_roz = Kf - Ki */
void calcularRozamiento(const MisionArtemis4 *m, ResultadoRozamiento *r) {
    double delta_h = m->h_inicial - m->h_paracaidas;

    r->ki = 0.5 * m->masa * m->v_inicial * m->v_inicial;
    r->kf = 0.5 * m->masa * m->v_paracaidas * m->v_paracaidas;
    r->trabajo_grav = m->masa * G_TERRESTRE * delta_h;
    r->trabajo_roz = (r->kf - r->ki) - r->trabajo_grav;
}

void calcularDesaceleracion(const MisionArtemis4 *m, ResultadoDesaceleracion *r) {
    r->delta_v = m->v_inicial - m->v_paracaidas;
    r->a_media = r->delta_v / m->dt_desaceleracion;
    r->g_multiplo = r->a_media / G_TERRESTRE;
}

/* En el agua: (F_prom - m*g) * d = 1/2 m v^2  ->  F_prom = m*g + K/d */
void calcularAmerizaje(const MisionArtemis4 *m, ResultadoAmerizaje *r) {
    r->peso = m->masa * G_TERRESTRE;
    r->energia_impacto = 0.5 * m->masa * m->v_impacto * m->v_impacto;
    r->fuerza_frenado = r->energia_impacto / m->profundidad_agua;
    r->f_prom = r->peso + r->fuerza_frenado;
    r->multiplo_peso = r->f_prom / r->peso;
}

EstadoAngulo clasificarAngulo(double angulo) {
    if (angulo > ANGULO_SEGURO_MAX) {
        return ANGULO_EMPINADO;
    }
    if (angulo < ANGULO_SEGURO_MIN) {
        return ANGULO_TANGENCIAL;
    }
    return ANGULO_SEGURO;
}

/*
 * Llena tres arreglos paralelos con n muestras equiespaciadas de la fase de
 * mayor desaceleracion (modelo de desaceleracion media constante):
 *   tiempos[i]      - instante [s]
 *   velocidades[i]  - rapidez  [m/s]
 *   disipada[i]     - % de la energia cinetica inicial ya disipada
 */
void generarTelemetria(const MisionArtemis4 *m, double tiempos[],
                       double velocidades[], double disipada[], int n) {
    ResultadoDesaceleracion d;
    int i;

    calcularDesaceleracion(m, &d);
    for (i = 0; i < n; i++) {
        tiempos[i] = m->dt_desaceleracion * i / (n - 1);
        velocidades[i] = m->v_inicial - d.a_media * tiempos[i];
        disipada[i] = (1.0 - (velocidades[i] * velocidades[i]) /
                             (m->v_inicial * m->v_inicial)) * 100.0;
    }
}

/*  */
/*  Presentacion                                                       */
/*  */

void mostrarParametros(MisionArtemis4 *mision) {
    ParametroEditable tabla[N_PARAMETROS];
    int n = obtenerParametros(mision, tabla, N_PARAMETROS);
    int i;

    mostrarEncabezado("PARAMETROS ACTUALES DE LA MISION");
    for (i = 0; i < n; i++) {
        printf("  %d. %-32s = %12.2f %s\n", i + 1, tabla[i].nombre,
               *tabla[i].valor, tabla[i].unidad);
    }
    printf("  g (gravedad terrestre, fija)       = %12.2f m/s^2\n", G_TERRESTRE);
}

void menuModificarParametros(MisionArtemis4 *mision) {
    ParametroEditable tabla[N_PARAMETROS];
    int n = obtenerParametros(mision, tabla, N_PARAMETROS);
    ParametroEditable *p;
    const char *motivo;
    char msg[160];
    double anterior;
    int op;
    int i;

    for (;;) {
        mostrarEncabezado("MODIFICAR PARAMETROS");
        for (i = 0; i < n; i++) {
            printf("  %d. %-32s = %12.2f %s\n", i + 1, tabla[i].nombre,
                   *tabla[i].valor, tabla[i].unidad);
        }
        printf("  0. Volver al menu principal\n");

        op = leerEntero("Parametro a modificar (0-9): ", 0, n);
        if (op == 0) {
            return;
        }

        p = &tabla[op - 1];
        snprintf(msg, sizeof(msg), "Nuevo valor de '%s' [%.2f a %.2f %s]: ",
                 p->nombre, p->minimo, p->maximo, p->unidad);
        anterior = *p->valor;
        *p->valor = leerDecimal(msg, p->minimo, p->maximo);

        if (!validarMision(mision, &motivo)) {
            *p->valor = anterior;   /* se revierte el cambio incoherente */
            mostrarAdvertencia(motivo);
            mostrarAdvertencia("Se conserva el valor anterior.");
        } else {
            printf("  Valor actualizado correctamente.\n");
        }
    }
}

void mostrarRozamiento(const MisionArtemis4 *mision) {
    ResultadoRozamiento r;
    calcularRozamiento(mision, &r);

    mostrarEncabezado("TRABAJO DE LA FUERZA DE ROZAMIENTO ATMOSFERICO");
    printf("  Planteo: W_grav + W_roz = Kf - Ki  ->  W_roz = (Kf - Ki) - m*g*(hi - hf)\n");
    mostrarSeparador();
    printf("  Energia cinetica inicial     Ki    = %.4e J\n", r.ki);
    printf("  Energia cinetica final       Kf    = %.4e J\n", r.kf);
    printf("  Trabajo de la gravedad       W_grav= %.4e J\n", r.trabajo_grav);
    printf("  Trabajo del rozamiento       W_roz = %.4e J\n", r.trabajo_roz);
    printf("  (Negativo: la fuerza de rozamiento se opone al movimiento y disipa\n");
    printf("   la energia como calor en el escudo termico. Se usa g constante.)\n");
}

void mostrarDesaceleracion(const MisionArtemis4 *mision) {
    ResultadoDesaceleracion r;
    double t[N_MUESTRAS_TELEMETRIA];
    double v[N_MUESTRAS_TELEMETRIA];
    double e[N_MUESTRAS_TELEMETRIA];
    int i;

    calcularDesaceleracion(mision, &r);
    generarTelemetria(mision, t, v, e, N_MUESTRAS_TELEMETRIA);

    mostrarEncabezado("DESACELERACION MEDIA EN LA FASE DE MAYOR FRENADO");
    printf("  Variacion de rapidez   = %.2f m/s\n", r.delta_v);
    printf("  Intervalo de tiempo    = %.2f s\n", mision->dt_desaceleracion);
    printf("  Desaceleracion media   = %.4f m/s^2\n", r.a_media);
    printf("  En multiplos de g      = %.2f g\n", r.g_multiplo);

    printf("\n  Telemetria (desaceleracion media constante):\n");
    printf("   %10s | %12s | %18s\n", "t [s]", "v [m/s]", "K disipada [%]");
    printf("   -----------+--------------+-------------------\n");
    for (i = 0; i < N_MUESTRAS_TELEMETRIA; i++) {
        printf("   %10.1f | %12.1f | %18.2f\n", t[i], v[i], e[i]);
    }
}

void mostrarAmerizaje(const MisionArtemis4 *mision) {
    ResultadoAmerizaje r;
    calcularAmerizaje(mision, &r);

    mostrarEncabezado("FUERZA PROMEDIO EN EL AMERIZAJE");
    printf("  Planteo: (F_prom - m*g) * d = 1/2 * m * v^2\n");
    mostrarSeparador();
    printf("  Peso de la capsula          = %.2f N\n", r.peso);
    printf("  Energia cinetica de impacto = %.2f J\n", r.energia_impacto);
    printf("  Profundidad de frenado      = %.2f m\n", mision->profundidad_agua);
    printf("  Fuerza neta de frenado K/d  = %.2f N\n", r.fuerza_frenado);
    printf("  Fuerza promedio del agua    = %.2f N\n", r.f_prom);
    printf("  Como multiplo del peso      = %.2f veces el peso\n", r.multiplo_peso);
}

void mostrarAnguloReingreso(const MisionArtemis4 *mision) {
    EstadoAngulo estado = clasificarAngulo(mision->angulo_reingreso);

    printf("\n  \n");
    printf("  ANALISIS DEL ANGULO DE REINGRESO (gamma = %.2f grados)\n",
           mision->angulo_reingreso);
    printf("   \n");
    printf("  Ventana de seguridad: %.1f <= gamma <= %.1f grados\n",
           ANGULO_SEGURO_MIN, ANGULO_SEGURO_MAX);

    switch (estado) {
        case ANGULO_SEGURO:
            printf("  [ESTADO: SEGURO] Dentro de la ventana. Disipacion termica y\n");
            printf("  fuerzas G controladas.\n");
            break;
        case ANGULO_EMPINADO:
            printf("  [ESTADO: PELIGRO - TRAYECTORIA MUY EMPINADA] (gamma > %.1f)\n",
                   ANGULO_SEGURO_MAX);
            printf("  -> La nave penetra rapido en capas densas: desaceleracion excesiva,\n");
            printf("     picos de fuerza G peligrosos para la tripulacion y calentamiento\n");
            printf("     aerodinamico que podria superar la capacidad del escudo termico.\n");
            break;
        case ANGULO_TANGENCIAL:
            printf("  [ESTADO: PELIGRO - TRAYECTORIA MUY TANGENCIAL] (gamma < %.1f)\n",
                   ANGULO_SEGURO_MIN);
            printf("  -> La nave no pierde suficiente energia: 'rebota' en la atmosfera\n");
            printf("     (skip) y vuelve al espacio, con perdida de la trayectoria de\n");
            printf("     amerizaje y riesgo critico para los suministros.\n");
            break;
    }
}

/*  */
/*  Simulacion por fases (juego)                                       */
/*  */

/* Agrega un evento al reporte (si hay lugar). */
static void registrarEvento(ReporteMision *rep, const char *fase, double t,
                            double h, double v, int superado) {
    EventoMision *e;

    if (rep->cantidad >= MAX_EVENTOS) {
        return;
    }
    e = &rep->eventos[rep->cantidad];
    snprintf(e->fase, sizeof(e->fase), "%s", fase);
    e->tiempo = t;
    e->altitud = h;
    e->velocidad = v;
    e->superado = superado;
    rep->cantidad++;
}

static void mostrarReporteFinal(const ReporteMision *rep, int mision_fallida) {
    int i;
    const char *veredicto;

    mostrarEncabezado("REPORTE FINAL DE DESEMPENO");
    printf("  %-3s %-28s %9s %11s %10s  %s\n",
           "N", "Fase", "t [s]", "h [m]", "v [m/s]", "Estado");
    mostrarSeparador();
    for (i = 0; i < rep->cantidad; i++) {
        printf("  %-3d %-28s %9.1f %11.1f %10.1f  %s\n", i + 1,
               rep->eventos[i].fase, rep->eventos[i].tiempo,
               rep->eventos[i].altitud, rep->eventos[i].velocidad,
               rep->eventos[i].superado ? "OK" : "PROBLEMAS");
    }
    mostrarSeparador();

    if (mision_fallida) {
        veredicto = "MISION FALLIDA - la nave no llego al amerizaje";
    } else if (rep->puntaje == 100) {
        veredicto = "MISION EXITOSA - tripulacion a salvo";
    } else if (rep->puntaje >= 60) {
        veredicto = "MISION CON RIESGOS - tripulacion recuperada con danos";
    } else {
        veredicto = "MISION CRITICA - supervivencia comprometida";
    }
    printf("  Puntaje: %d / 100\n", rep->puntaje);
    printf("  Veredicto: %s\n", veredicto);
}

void ejecutarSimulacion(const MisionArtemis4 *base) {
    MisionArtemis4 m = *base;          /* copia: la simulacion no altera la base */
    ReporteMision rep;
    ResultadoDesaceleracion des;
    EstadoAngulo estado;
    char op;
    double t_amerizaje;
    int fallida = 0;

    memset(&rep, 0, sizeof(rep));
    rep.puntaje = 100;

    mostrarEncabezado("SIMULACION: REINGRESO Y AMERIZAJE DE ORION");
    printf("  Modulo de tripulacion: %.1f kg, reingresando a %.1f m/s desde %.1f m.\n",
           m.masa, m.v_inicial, m.h_inicial);

    /*  Turno 1: reingreso  */
    printf("\n>>> TURNO 1/4: INTERFAZ ATMOSFERICA\n");
    printf("  Angulo de reingreso programado: %.2f grados (ventana %.1f-%.1f).\n",
           m.angulo_reingreso, ANGULO_SEGURO_MIN, ANGULO_SEGURO_MAX);
    op = leerCaracter("  Desea cambiar el angulo para esta simulacion? (s/n): ", "sn");
    if (op == 's') {
        m.angulo_reingreso = leerDecimal("  Nuevo angulo de reingreso [0.1 a 90.0 grados]: ",
                                         0.1, 90.0);
    }

    estado = clasificarAngulo(m.angulo_reingreso);
    registrarEvento(&rep, "Reingreso atmosferico", 0.0, m.h_inicial, m.v_inicial,
                    estado == ANGULO_SEGURO);
    mostrarAnguloReingreso(&m);

    if (estado != ANGULO_SEGURO) {
        fallida = 1;
        rep.puntaje = 0;
        printf("\n  *** La nave no puede completar el reingreso. ***\n");
    }

    /*  Turno 2: frenado maximo  */
    if (!fallida) {
        esperarEnter("\n  Presione ENTER para continuar al turno 2...");
        printf("\n>>> TURNO 2/4: FASE DE MAYOR DESACELERACION\n");
        calcularDesaceleracion(&m, &des);
        mostrarDesaceleracion(&m);

        if (des.g_multiplo > LIMITE_G_TRIPULACION) {
            printf("\n  [ALERTA] %.2f g supera el limite de %.1f g tolerable por la tripulacion.\n",
                   des.g_multiplo, LIMITE_G_TRIPULACION);
            rep.puntaje -= 30;
        }
        registrarEvento(&rep, "Frenado maximo", m.dt_desaceleracion, m.h_paracaidas,
                        m.v_paracaidas, des.g_multiplo <= LIMITE_G_TRIPULACION);
    }

    /*  Turno 3: paracaidas  */
    if (!fallida) {
        esperarEnter("\n  Presione ENTER para continuar al turno 3...");
        printf("\n>>> TURNO 3/4: APERTURA DE PARACAIDAS PRINCIPALES\n");
        mostrarRozamiento(&m);
        registrarEvento(&rep, "Apertura de paracaidas", m.dt_desaceleracion,
                        m.h_paracaidas, m.v_paracaidas, 1);
    }

    /*  Turno 4: amerizaje  */
    if (!fallida) {
        esperarEnter("\n  Presione ENTER para continuar al turno 4...");
        printf("\n>>> TURNO 4/4: AMERIZAJE EN EL OCEANO PACIFICO\n");
        mostrarAmerizaje(&m);

        /* Tiempo estimado de descenso con desaceleracion uniforme. */
        t_amerizaje = m.dt_desaceleracion +
                      m.h_paracaidas / ((m.v_paracaidas + m.v_impacto) / 2.0);

        if (m.v_impacto > V_IMPACTO_SEGURA) {
            printf("\n  [ALERTA] Impacto a %.1f m/s: supera los %.1f m/s considerados seguros.\n",
                   m.v_impacto, V_IMPACTO_SEGURA);
            rep.puntaje -= 40;
        }
        registrarEvento(&rep, "Amerizaje (t estimado)", t_amerizaje, 0.0, m.v_impacto,
                        m.v_impacto <= V_IMPACTO_SEGURA);
    }

    esperarEnter("\n  Presione ENTER para ver el reporte final...");
    mostrarReporteFinal(&rep, fallida);
}
