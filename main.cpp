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
    SalaEspera3 salaEspera3;
    Hospital hospital;
    atomic<bool> finalizarMedicos(false);
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
    thread enfermeiraThread1(&Enfermeira::chamarPacienteDaTriagem, &enfermeira1, ref(salaEspera2), ref(salaEspera3));
    thread enfermeiraThread2(&Enfermeira::chamarPacienteDaTriagem, &enfermeira2, ref(salaEspera2), ref(salaEspera3));

    // Thread para médico clínico
thread clinicoThread([&hospital, &finalizarMedicos, &salaEspera3]() {
    while (!finalizarMedicos || !hospital.filaClinicosVazia()) {
        cout << "Clínico verificando fila..." << endl;
        if (!hospital.filaClinicosVazia()) {
            Paciente paciente = hospital.getProximoPacienteClinico();
            cout << "Clínico atendendo paciente " << paciente.id << " - Prioridade: " << paciente.prioridadeAtendimento << endl;
            // Simular atendimento
            this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 500)); // Tempo de atendimento aleatório
            paciente.gerarDiagnostico(); // Gerar diagnóstico
        } else {
            cout << "Fila de Clínicos vazia, aguardando pacientes..." << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
});

// Thread para médico ortopedista
thread ortopedistaThread([&hospital, &finalizarMedicos, &salaEspera3]() {
   while (!finalizarMedicos || !hospital.filaOrtopedistasVazia()) {
        cout << "Ortopedista verificando fila..." << endl;
        if (!hospital.filaOrtopedistasVazia()) {
            Paciente paciente = hospital.getProximoPacienteOrtopedista();
            cout << "Ortopedista atendendo paciente " << paciente.id << " - Prioridade: " << paciente.prioridadeAtendimento << endl;
            // Simular atendimento
            this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 500)); // Tempo de atendimento aleatório
            paciente.gerarDiagnostico(); // Gerar diagnóstico
        } else {
            cout << "Fila de Ortopedistas vazia, aguardando pacientes..." << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
});

    // Esperar que todas as threads dos pacientes terminem
    for (auto& th : threadsPacientes) {
        pthread_join(th, nullptr);
    }

    // Encerrar threads dos atendentes e enfermeiras
    atendenteThread1.join();
    atendenteThread2.join();

    // Marcar atendentes como terminados
    salaEspera2.marcarAtendentesTerminados();

    while (!salaEspera2.salaVazia()) {
    this_thread::sleep_for(chrono::seconds(1));
}

    finalizarMedicos = true;
    clinicoThread.join();
    ortopedistaThread.join();

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