#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <pthread.h> // Para usar pthread
#include <semaphore.h> // Para usar semáforos
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <atomic> 



using namespace std;

class Paciente {
private:
    // Semáforo para controlar o acesso à função retirarSenha
    sem_t semaforoRetiradaSenha;
public:
    int id;
    string nome;
    int idade;
    string motivoVisita;
    string tipoPrioridade; // Normal ou prioritário
    bool temNecessidadesEspeciais;
    bool mulherComFilhoDeColo;
    int numeroSenha;
    std::string momentoRetiradaSenha;

    

    Paciente() {
        sem_init(&semaforoRetiradaSenha, 0, 1); // Inicialização do semáforo com 1 (indicando acesso exclusivo)
    }

    void coletarInformacoes() {
        cout << "Coletando informações do paciente " << id << "." << endl;
        
        // Simulação da coleta de informações. Em um cenário real, esses dados viriam de um formulário ou seriam inseridos por um funcionário.
        nome = "Paciente_" + to_string(id);
        idade = rand() % 100; // Idade aleatória entre 0 e 99
        motivoVisita = "Motivo_" + to_string(rand() % 5); // Motivo fictício

        cout << "Nome: " << nome << ", Idade: " << idade << ", Motivo da Visita: " << motivoVisita << endl;

        // Aqui você pode adicionar mais lógica se necessário.
    }

    // Método para retirar a senha
    void retirarSenha(int numero) {
        sem_wait(&semaforoRetiradaSenha); // Espera pelo semáforo

        numeroSenha = numero; // Definindo o número da senha do paciente
        cout << "Paciente " << id << " retirou a senha " << numeroSenha << endl;

        // Obter o momento atual como um time_point
        auto now = std::chrono::system_clock::now();

        // Obter o tempo atual como um tempo de calendário
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        // Obter os milissegundos
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // Converter para uma estrutura de tempo local para formatação
        std::tm *localTime = std::localtime(&currentTime);

        // Formatar a data e hora com os milissegundos
        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

        // Armazenar o tempo formatado na string momentoRetiradaSenha
        momentoRetiradaSenha = oss.str();

        sem_post(&semaforoRetiradaSenha); // Libera o semáforo após a operação ser concluída
    }

    // Método para imprimir senha e os detalhes do paciente
    void imprimirDetalhesSenha(){
        cout << "Paciente: " << id << ", Prioridade: " << tipoPrioridade << ", Senha: " << numeroSenha;
        if (temNecessidadesEspeciais)
            cout << ", Necessidades Especiais: Sim";
        if (mulherComFilhoDeColo)
            cout << ", Mulher com Filho de Colo: Sim";
        // Imprimir outros detalhes do paciente, se necessário

        cout << ", Momento da retirada: " << momentoRetiradaSenha;
        cout << endl;
    }

    ~Paciente() {
        sem_destroy(&semaforoRetiradaSenha); // Destruir o semáforo
    }
};







class Medico {
public:
    // Métodos e atributos dos médicos podem ser adicionados aqui
};


class SalaEspera1 {
private:
    vector<Paciente> pacientesEsperando;
    mutable mutex mtx; // mutex para sincronização
public:
    int maxSize = 50;

    void adicionarPaciente(const Paciente& paciente) {
        std::lock_guard<std::mutex> guard(mtx);

        if (pacientesEsperando.size() >= maxSize) {
            std::cout << "Sala de Espera 1 está cheia." << std::endl;
        } else {
            pacientesEsperando.push_back(paciente);
            std::cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 1." << std::endl;
        }
    }

    void imprimirQuantidadePacientes() {
        lock_guard<mutex> guard(mtx);
        std::cout << "Quantidade de pacientes na Sala de Espera 1: " << pacientesEsperando.size() << std::endl;
    }

    bool salaCheia() {
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.size() >= maxSize;
    }

    bool salaVazia(){
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.size() == 0;
    }

    Paciente getPrioridade() {
        lock_guard<mutex> guard(mtx);
        std::sort(pacientesEsperando.begin(), pacientesEsperando.end(), [](const Paciente& p1, const Paciente& p2) {
            // Primeiro, ordene por prioridade
            if (p1.tipoPrioridade == "Prioritario" && p2.tipoPrioridade != "Prioritario") {
                return true;
            } else if (p1.tipoPrioridade != "Prioritario" && p2.tipoPrioridade == "Prioritario") {
                return false;
            } else {
                // Se ambos forem prioritários ou não prioritários, ordene com base no id (ordem de chegada)
                return p1.id < p2.id;
            }
        });


        // Retorna o paciente mais prioritário (primeiro da lista ordenada)
        return pacientesEsperando.front();
    }

    void removerPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);
        auto it = std::remove_if(pacientesEsperando.begin(), pacientesEsperando.end(), [&paciente](const Paciente& p) {
            return p.id == paciente.id;
        });

        if (it != pacientesEsperando.end()) {
            pacientesEsperando.erase(it, pacientesEsperando.end());
            std::cout << "Paciente " << paciente.id << " removido da Sala de Espera 1." << std::endl;
        } else {
            std::cout << "Paciente " << paciente.id << " não encontrado na Sala de Espera 1." << std::endl;
        }
    }



};

class SalaEspera2 {
private:
    vector<Paciente> pacientesEsperando;
    mutex mtx; // mutex para sincronização
public:
    int maxSize = 50;

    void adicionarPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);

        if (pacientesEsperando.size() >= maxSize) {
            std::cout << "Sala de Espera 2 está cheia." << std::endl;
        } else {
            pacientesEsperando.push_back(paciente);
            std::cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 2." << std::endl;
        }
    }

    Paciente getPrioridade() {
        lock_guard<mutex> guard(mtx);
        std::sort(pacientesEsperando.begin(), pacientesEsperando.end(), [](const Paciente& p1, const Paciente& p2) {
            // Primeiro, ordene por prioridade
            if (p1.tipoPrioridade == "Prioritario" && p2.tipoPrioridade != "Prioritario") {
                return true;
            } else if (p1.tipoPrioridade != "Prioritario" && p2.tipoPrioridade == "Prioritario") {
                return false;
            } else {
                // Se ambos forem prioritários ou não prioritários, ordene com base no id (ordem de chegada)
                return p1.id < p2.id;
            }
        });


        // Retorna o paciente mais prioritário (primeiro da lista ordenada)
        return pacientesEsperando.front();
    }

    void imprimirQuantidadePacientes() {
        lock_guard<mutex> guard(mtx);
        std::cout << "Quantidade de pacientes na Sala de Espera 2: " << pacientesEsperando.size() << std::endl;
    }

    bool salaCheia() {
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.size() >= maxSize;
    }

    bool salaVazia(){
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.size() == 0;
    }

    void removerPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);
        auto it = std::remove_if(pacientesEsperando.begin(), pacientesEsperando.end(), [&paciente](const Paciente& p) {
            return p.id == paciente.id;
        });

        if (it != pacientesEsperando.end()) {
            pacientesEsperando.erase(it, pacientesEsperando.end());
            std::cout << "Paciente " << paciente.id << " removido da Sala de Espera 2." << std::endl;
        } else {
            std::cout << "Paciente " << paciente.id << " não encontrado na Sala de Espera 2." << std::endl;
        }
    }

};


class Atendente {
private:
    sem_t semaforoAtendimento;

public:
    Atendente() {
        sem_init(&semaforoAtendimento, 0, 1); // semáforo para controle de acesso
    }

    void chamarPaciente(SalaEspera2& salaEspera2, SalaEspera1& salaEspera1) {
    while (true) {
            if (salaEspera1.salaVazia()) {
                break;
            }
            if (salaEspera2.salaCheia()) {
                continue; // Aguarda até que haja espaço na SalaEspera2
            }
            Paciente paciente = salaEspera1.getPrioridade();
            salaEspera1.removerPaciente(paciente);
            
            int tempoAtendimento = (rand() % 3) + 1;
            this_thread::sleep_for(chrono::seconds(tempoAtendimento));

            salaEspera2.adicionarPaciente(paciente);
        }
}



    ~Atendente() {
        sem_destroy(&semaforoAtendimento); // destruir o semáforo
    }
};



class Enfermeira {
// private:
//     atomic<bool> finalizarThread{false};    
// public:
//     void chamarPacienteDaTriagem(SalaEspera2& salaEspera2) {
//         while (!finalizarThread) {
//             if (salaEspera2.salaVazia()) {
//                 this_thread::sleep_for(chrono::seconds(1));
//                 continue;
//             }

//             Paciente paciente = salaEspera2.getPrioridade();
//             cout << "Enfermeira chamando paciente " << paciente.id << " para triagem." << endl;
//             this_thread::sleep_for(chrono::seconds(1));
//             salaEspera2.removerPaciente(paciente);
//         }
//     }

//     void finalizar() {
//         finalizarThread = true;
//     }
};