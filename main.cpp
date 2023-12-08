#include <vector>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include "classes.h"
#include "workers.h"

// Definição do número total de pacientes
const int NUM_PACIENTES = 10;

using namespace std;

int main() {

    std::vector<pthread_t> threadsPacientes;
    std::vector<Paciente> pacientes;
    SalaEspera1 salaEspera1;
    SalaEspera2 salaEspera2;

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

    salaEspera1.getPrioridade();

    for (auto& thread : threadsPacientes) {
        pthread_join(thread, nullptr); // Esperar que todas as threads dos pacientes terminem
    }

    // Criar instâncias dos atendentes
    Atendente atendente1;

    // Simular chamada de pacientes pelos atendentes
    std::thread atendenteThread1(&Atendente::chamarPaciente, &atendente1, std::ref(salaEspera2), std::ref(salaEspera1));


    // Esperar que as threads dos atendentes terminem
    atendenteThread1.join();

    // Imprimir quantidade de pacientes nas salas de espera após as chamadas
    salaEspera1.imprimirQuantidadePacientes();
    salaEspera2.imprimirQuantidadePacientes();


    return 0;
}
