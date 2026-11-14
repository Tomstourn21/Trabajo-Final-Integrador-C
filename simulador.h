#ifndef SIMULADOR_H
#define SIMULADOR_H

typedef struct {
    float masa;            
    float v_inicial;      
    float h_inicial;        
    float v_paracaidas;     
    float h_paracaidas;     
    float dt_desaceleracion;
    float v_impacto;        
    float profundidad_agua; 
    float angulo_reingreso; 
} MisionArtemis4;

void inicializarMision(MisionArtemis4 *mision);
void calcularTrabajoRozamiento(MisionArtemis4 mision);
void calcularDesaceleracionMedia(MisionArtemis4 mision);
void calcularFuerzaAmerizaje(MisionArtemis4 mision);
void analizarAnguloReingreso(MisionArtemis4 mision);
void mostrarReporteCompleto(MisionArtemis4 mision);

#endif