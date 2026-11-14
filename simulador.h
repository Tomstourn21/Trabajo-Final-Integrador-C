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

void DatosMision(MisionArtemis4 *mision);
void calculodeRozamiento(MisionArtemis4 mision);
void calculodeDesaceleracion(MisionArtemis4 mision);
void calculodeFuerzaAmerizaje(MisionArtemis4 mision);
void analizarAngulodeReingreso(MisionArtemis4 mision);

#endif
