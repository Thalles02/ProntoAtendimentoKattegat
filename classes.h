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




class Paciente;


// Definição das classes Clinico e Ortopedista
class Clinico {
public:
    void atenderPaciente(Paciente& paciente);
};

class Ortopedista {
public:
    void atenderPaciente(Paciente& paciente);
};


class Paciente {
private:
    // Semáforo para controlar o acesso à função retirarSenha
    sem_t semaforoRetiradaSenha;

public:
    float temperatura;
    int batimentoCardiacos;
    Clinico clinico;
    Ortopedista ortopedista;
    string prioridadeAtendimento;
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
        cout << endl << "Coletando informações do paciente " << id << "." << endl;
        
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
        cout << endl << endl;
    }

    void avaliarSinaisVitais(Clinico& clinico, Ortopedista& ortopedista);
};


// Implementação dos métodos de Clinico e Ortopedista
void Clinico::atenderPaciente(Paciente& paciente) {
    cout << "Clínico atendendo paciente " << paciente.id << endl;
}

void Ortopedista::atenderPaciente(Paciente& paciente) {
    cout << "Ortopedista atendendo paciente " << paciente.id << endl;
}

void Paciente::avaliarSinaisVitais(Clinico& clinico, Ortopedista& ortopedista) {
        // Simular a avaliação dos sinais vitais

        temperatura = (rand() % 5) + 36; // Temperatura aleatória entre 36 e 40
        batimentoCardiacos = (rand() % 61) + 60; // Batimentos cardíacos aleatórios entre 60 e 120

        // Determinar a prioridade de atendimento baseado em alguma lógica
        if (temperatura > 38 || batimentoCardiacos > 100) {
            prioridadeAtendimento = "Emergência";
        } else if (temperatura > 37 || batimentoCardiacos > 80) {
            prioridadeAtendimento = "Grave";
        } else {
            prioridadeAtendimento = "Normal";
        }

        if (rand() % 2 == 0) {
                        clinico.atenderPaciente(*this);
                    } else {
                        ortopedista.atenderPaciente(*this);
                    }
    }





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
        cout << "Sala de Espera 1 está cheia. Paciente " << paciente.id << " não pode entrar." << endl;
    } else {
        pacientesEsperando.push_back(paciente);
        cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 1. Tipo: " << (paciente.tipoPrioridade == "Prioritario" ? "Prioritário" : "Normal") << endl;
    }
    }

    void imprimirQuantidadePacientes() {
        lock_guard<mutex> guard(mtx);
        std::cout << "Quantidade de pacientes na Sala de Espera 1: " << pacientesEsperando.size() << std::endl << endl;
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
        if (pacientesEsperando.empty()) {
            throw std::runtime_error("Sala de espera vazia.");
        }
        std::sort(pacientesEsperando.begin(), pacientesEsperando.end(), [](const Paciente& p1, const Paciente& p2) {
        if (p1.tipoPrioridade != p2.tipoPrioridade) {
            return p1.tipoPrioridade == "Prioritario";
        }
        return p1.id < p2.id;
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
            std::cout << "Paciente " << paciente.id << " removido da Sala de Espera 1." << std::endl << endl;
        } else {
            std::cout << "Paciente " << paciente.id << " não encontrado na Sala de Espera 1." << std::endl << endl;
        }
    }



};

class SalaEspera2 {
private:
    vector<Paciente> pacientesEsperando;
    mutex mtx; // Mutex para sincronização
    atomic<bool> atendentesTerminaramFlag{false};
public:
    int maxSize = 50;

    void adicionarPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);

        if (pacientesEsperando.size() >= maxSize) {
            std::cout << "Sala de Espera 2 está cheia." << std::endl << endl;
        } else {
            pacientesEsperando.push_back(paciente);
            std::cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 2." << std::endl << endl;
        }
    }


    void marcarAtendentesTerminados() {
        atendentesTerminaramFlag = true;
    }

    bool atendentesTerminaram() const {
        return atendentesTerminaramFlag.load();
    }

    bool processamentoCompleto() {
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.empty() && atendentesTerminaramFlag;
    }

    Paciente getPrioridade() {
        lock_guard<mutex> guard(mtx);
        if (pacientesEsperando.empty()) {
            throw std::runtime_error("Sala de espera vazia.");
        }
        std::sort(pacientesEsperando.begin(), pacientesEsperando.end(), [](const Paciente& p1, const Paciente& p2) {
            if (p1.tipoPrioridade != p2.tipoPrioridade) {
                return p1.tipoPrioridade == "Prioritario";
            }
            return p1.id < p2.id;
        });

        return pacientesEsperando.front();
    }

    void imprimirQuantidadePacientes() {
        lock_guard<mutex> guard(mtx);
        std::cout << "Quantidade de pacientes na Sala de Espera 2: " << pacientesEsperando.size() << std::endl << endl;
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
            std::cout << "Paciente " << paciente.id << " removido da Sala de Espera 2." << std::endl << endl;
        } else {
            std::cout << "Paciente " << paciente.id << " não encontrado na Sala de Espera 2." << std::endl << endl;
        }
    }

};

class Enfermeira {
private:
    atomic<bool> finalizarThread{false};    
    atomic<bool>& notificacao;

public:


    Enfermeira(atomic<bool>& notif) : notificacao(notif) {}

    void chamarPacienteDaTriagem(SalaEspera2& salaEspera2) {
        Clinico clinico;
        Ortopedista ortopedista;
        while (!finalizarThread) {
            if (!salaEspera2.salaVazia()) {
                try {
                    
                    Paciente paciente = salaEspera2.getPrioridade();
                    paciente.avaliarSinaisVitais(clinico, ortopedista);
                    cout << "Enfermeira chamando paciente " << paciente.id << " para triagem." << endl;
                    salaEspera2.removerPaciente(paciente);

                    if (!salaEspera2.salaVazia()) {
                    Paciente paciente = salaEspera2.getPrioridade();
                    

                    // Encaminhar para clínico ou ortopedista
                    
            }
                } catch (const std::runtime_error& e) {
                    // Tratamento de erro para quando a sala estiver vazia
                    cout << e.what() << endl;
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            } else if (!salaEspera2.atendentesTerminaram()) {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
    }

    void finalizar() {
        finalizarThread = true;
        notificacao = true; // Certifique-se de acordar a enfermeira se ela estiver esperando
    }
};







class Atendente {
private:
    sem_t semaforoAtendimento;
    atomic<bool>& notificacao; // Adicione esta linha

public:
    // Construtor modificado para aceitar a referência da variável de notificação
    Atendente(atomic<bool>& notif) : notificacao(notif) {
        sem_init(&semaforoAtendimento, 0, 1); // Semáforo para controle de acesso
    }

    void chamarPaciente(SalaEspera2& salaEspera2, SalaEspera1& salaEspera1) {
    while (true) {
        if (salaEspera1.salaVazia()) {
            break;
        }
        if (salaEspera2.salaCheia()) {
            continue;
        }
        Paciente paciente = salaEspera1.getPrioridade();
        salaEspera1.removerPaciente(paciente);
        
        int tempoAtendimento = (rand() % 3) + 1;
        this_thread::sleep_for(chrono::seconds(tempoAtendimento));

        salaEspera2.adicionarPaciente(paciente);
        cout << "Atendente chamando paciente " << paciente.id << ". Tipo: " << (paciente.tipoPrioridade == "Prioritario" ? "Prioritário" : "Normal") << endl;
        notificacao = true; // Sinalizar as enfermeiras
    }
    salaEspera2.marcarAtendentesTerminados();
}


    ~Atendente() {
        sem_destroy(&semaforoAtendimento); // Destruir o semáforo
    }
};




