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


using namespace std;

class Paciente {
private:
    // Semáforo para controlar o acesso à função retirarSenha
    sem_t semaforoRetiradaSenha;
public:
    int id;
    string tipoPrioridade; // Normal ou prioritário
    bool temNecessidadesEspeciais;
    bool mulherComFilhoDeColo;
    int numeroSenha;
    std::string momentoRetiradaSenha;

    

    Paciente() {
        sem_init(&semaforoRetiradaSenha, 0, 1); // Inicialização do semáforo com 1 (indicando acesso exclusivo)
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


class Atendente {
public:
    
    void realizaCadastro(){

    }

};


class Enfermeira {
public:
    // Métodos e atributos das enfermeiras podem ser adicionados aqui
};

class Medico {
public:
    // Métodos e atributos dos médicos podem ser adicionados aqui
};


class SalaEspera1 {
private:
    std::vector<Paciente> pacientesEsperando;
public:
    int maxSize = 50;

    void adicionarPaciente(const Paciente& paciente) {

        if (pacientesEsperando.size() >= maxSize) {
            std::cout << "Sala de Espera 1 está cheia." << std::endl;
        } else {
            pacientesEsperando.push_back(paciente);
            std::cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 1." << std::endl;
        }
    }

    void imprimirQuantidadePacientes() {
        std::cout << "Quantidade de pacientes na Sala de Espera 1: " << pacientesEsperando.size() << std::endl;
    }

    bool salaCheia() {
        return pacientesEsperando.size() >= maxSize;
    }

    bool salaVazia(){
        return pacientesEsperando.size() < maxSize;
    }

    Paciente getPrioridade() {
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

        std::cout << "Pacientes ordenados por prioridade:" << std::endl;
        for (size_t i = 0; i < pacientesEsperando.size(); ++i) {
            const Paciente& paciente = pacientesEsperando[i];
            std::cout << "Índice: " << i << ", ID: " << paciente.id << ", Prioridade: " << paciente.tipoPrioridade << std::endl;
            // Imprima outras informações relevantes do paciente, se necessário
        }

        // Retorna o paciente mais prioritário (primeiro da lista ordenada)
        return pacientesEsperando.front();
    }


};

class SalaEspera2 {
private:
    std::vector<Paciente> pacientesEsperando;
public:
    int maxSize = 50;

    bool salaVazia(){
        return pacientesEsperando.size() < maxSize;
    }

};