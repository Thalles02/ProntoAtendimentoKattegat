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

void simularChegadaPacientes() {
    std::vector<pthread_t> threadsPacientes;
    std::vector<Paciente> pacientes;
    SalaEspera1 salaEspera1;

    for (int i = 1; i <= 10; ++i) {
        Paciente paciente;
        paciente.id = i;
        
        randomicLevel(paciente);

        paciente.retirarSenha(i); // Simular a retirada da senha
        paciente.imprimirDetalhesSenha(); // Imprimir os detalhes do paciente e da senha

        pacientes.push_back(paciente);

        salaEspera1.adicionarPaciente(paciente);

        // Verificar se a sala de espera está cheia e imprimir a quantidade de pacientes
        if (salaEspera1.salaCheia()) {
            std::cout << "Sala de Espera 1 está cheia." << std::endl;
        } else {
            salaEspera1.imprimirQuantidadePacientes();
        }

        
        pthread_t thread;
        pthread_create(&thread, nullptr, comportamentoPaciente, static_cast<void*>(&pacientes.back()));
        threadsPacientes.push_back(thread);
    }

    for (auto& thread : threadsPacientes) {
        pthread_join(thread, nullptr); // Esperar que todas as threads dos pacientes terminem
    }
}

