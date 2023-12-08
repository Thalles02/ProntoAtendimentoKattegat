#include "classes.h"
#include <vector>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime> 

void* comportamentoPaciente(void* paciente_void_ptr) {
    Paciente* paciente_ptr = static_cast<Paciente*>(paciente_void_ptr);

    // Aqui você pode adicionar a lógica de comportamento do paciente
    // Por exemplo, aguardar, ser atendido, etc.

    return nullptr;
}

void randomicLevel(Paciente& paciente) {
    int level = rand() % 3; // 0 - Normal, 1 - Necessidades especiais, 2 - Mulher de colo
    if (level == 0) {
        paciente.tipoPrioridade = "Normal";
        paciente.temNecessidadesEspeciais = false;
        paciente.mulherComFilhoDeColo = false;
    } else if (level == 1) {
        paciente.tipoPrioridade = "Prioritario";
        paciente.temNecessidadesEspeciais = true;
        paciente.mulherComFilhoDeColo = false;
    } else {
        paciente.tipoPrioridade = "Prioritario";
        paciente.temNecessidadesEspeciais = false;
        paciente.mulherComFilhoDeColo = true;
    }
}


