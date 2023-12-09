#include <vector>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include "classes.h"
#include "workers.h"

using namespace std;

const int NUM_PACIENTES = 20;

int main() {
    vector<pthread_t> threadsPacientes;
    vector<Paciente> pacientes;
    SalaEspera1 salaEspera1;
    SalaEspera2 salaEspera2;
    atomic<bool> notificacao(false);

    // Instância das enfermeiras e atendentes
    Enfermeira enfermeira1(notificacao), enfermeira2(notificacao);
    Atendente atendente1(notificacao), atendente2(notificacao);

    // Processamento de pacientes
    for (int i = 1; i <= NUM_PACIENTES; ++i) {
        Paciente paciente;
        paciente.id = i;
        
        randomicLevel(paciente);
        paciente.coletarInformacoes();
        paciente.retirarSenha(i);
        paciente.imprimirDetalhesSenha();

        pacientes.push_back(paciente);
        salaEspera1.adicionarPaciente(paciente);

        pthread_t thread;
        pthread_create(&thread, nullptr, comportamentoPaciente, static_cast<void*>(&pacientes.back()));
        threadsPacientes.push_back(thread);
    }

    // Threads dos atendentes
    thread atendenteThread1(&Atendente::chamarPaciente, &atendente1, ref(salaEspera2), ref(salaEspera1));
    thread atendenteThread2(&Atendente::chamarPaciente, &atendente2, ref(salaEspera2), ref(salaEspera1));

    // Threads das enfermeiras
    thread enfermeiraThread1(&Enfermeira::chamarPacienteDaTriagem, &enfermeira1, ref(salaEspera2));
    thread enfermeiraThread2(&Enfermeira::chamarPacienteDaTriagem, &enfermeira2, ref(salaEspera2));

    // Esperar que todas as threads dos pacientes terminem
    for (auto& th : threadsPacientes) {
        pthread_join(th, nullptr);
    }

    // Encerrar threads dos atendentes e enfermeiras
    atendenteThread1.join();
    atendenteThread2.join();

    // Marcar como finalizado
    // salaEspera2.marcarFinalizado();

    // Encerrar threads das enfermeiras
    enfermeira1.finalizar();
    enfermeira2.finalizar();
    enfermeiraThread1.join();
    enfermeiraThread2.join();

    // Imprimir a quantidade de pacientes nas salas de espera
    salaEspera1.imprimirQuantidadePacientes();
    salaEspera2.imprimirQuantidadePacientes();

    return 0;
}
