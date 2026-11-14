#include <stdio.h>
#include <math.h>
#include "simulador.h"

#define G_TERRESTRE 9.80f

void DatosMision(MisionArtemis4 *mision) {
    mision->masa = 10400.0f;
    mision->v_inicial = 11000.0f;
    mision->h_inicial = 120000.0f;
    mision->v_paracaidas = 25.0f;
    mision->h_paracaidas = 2500.0f;
    mision->dt_desaceleracion = 400.0f;
    mision->v_impacto = 8.0f;
    mision->profundidad_agua = 1.50f;
    mision->angulo_reingreso = 6.20f;
}

void calculodeRozamiento(MisionArtemis4 mision) {
    float ki = 0.5f * mision.masa * powf(mision.v_inicial, 2.0f);
    float kf = 0.5f * mision.masa * powf(mision.v_paracaidas, 2.0f);
    float delta_k = kf - ki;
    
    float delta_h = mision.h_inicial - mision.h_paracaidas;
    float trabajo_grav = mision.masa * G_TERRESTRE * delta_h;
    
    float trabajo_roz = delta_k - trabajo_grav;
    
    printf("\n=== TRABAJO DE LA FUERZA DE ROZAMIENTO ATMOSFERICO ===\n");
    printf(" - Energia Cinetica Inicial= %.4e J\n", ki);
    printf(" - Energia Cinetica Final= %.4e J\n", kf);
    printf(" - Trabajo de la Gravedad= %.4e J\n", trabajo_grav);
    printf(" - Trabajo Total de Rozamiento= %.4e J\n", trabajo_roz);
    printf("   *(Valor negativo ya que la fuerza opone resistencia al avance)*\n");
}

void calculodeDesaceleracion(MisionArtemis4 mision) {
    float delta_v = mision.v_inicial - mision.v_paracaidas;
    float a_media = delta_v / mision.dt_desaceleracion;
    float g_multiplo = a_media / G_TERRESTRE;
    
    printf("\n=== DESACELERACION MEDIA EN LA FASE PRINCIPAL ===\n");
    printf(" - Variacion de Velocidad= %.2f m/s\n", delta_v);
    printf(" - Intervalo de Tiempo= %.2f s\n", mision.dt_desaceleracion);
    printf(" - Desaceleracion Media= %.4f m/s^2\n", a_media);
    printf(" - Equivalente en multiplos de gravedad= %.2f g\n", g_multiplo);
}

void calculodeFuerzaAmerizaje(MisionArtemis4 mision) {
    float peso = mision.masa * G_TERRESTRE;
    float energia_cinetica_impacto = 0.5f * mision.masa * powf(mision.v_impacto, 2.0f);
    float fuerza_extra = energia_cinetica_impacto / mision.profundidad_agua;
    float f_prom = peso + fuerza_extra;
    float multiplo_peso = f_prom / peso;
    
    printf("\n=== FUERZA PROMEDIO EN EL AMERIZAJE ===\n");
    printf(" - Peso de la capsula= %.2f N\n", peso);
    printf(" - Profundidad de frenado en el agua= %.2f m\n", mision.profundidad_agua);
    printf(" - Fuerza Promedio del Agua= %.2f N\n", f_prom);
    printf(" - Expresado como multiplo del peso= %.2f veces el peso\n", multiplo_peso);
}

void analizarAngulodeReingreso(MisionArtemis4 mision) {
    printf("\n=== ANALISIS DEL ANGULO DE REINGRESO (gamma = %.2f°) ===\n", mision.angulo_reingreso);
    printf("Ventana de seguridad normada: 5.5° <= gamma <= 7.0°\n");
    
    if (mision.angulo_reingreso >= 5.5f && mision.angulo_reingreso <= 7.0f) {
        printf(" [ESTADO: SEGURO] Dentro del rango optimo. Disipacion termica y fuerzas G controladas.\n");
    } else if (mision.angulo_reingreso > 7.0f) {
        printf(" [ESTADO: PELIGRO - TRAYECTORIA MUY EMPINADA]\n");
        printf(" -> Consecuencias: Desaceleracion excesiva y picos extremos de fuerzas G sobre la tripulacion,\n");
        printf("    sumado a un calentamiento aerodinamico que podria fundir el escudo termico.\n");
    } else {
        printf(" [ESTADO: PELIGRO - TRAYECTORIA MUY TANGENCIAL]\n");
        printf(" -> Consecuencias: La nave no logra frenar adecuadamente y experimenta un 'rebote atmosferico'\n");
        printf("    (skip), rebotando de regreso al espacio exterior con perdida critica de suministros.\n");
    }
}
