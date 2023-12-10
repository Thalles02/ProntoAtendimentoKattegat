#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <atomic>

using namespace std;

// Declarações antecipadas
class Paciente;
class Clinico;
class Ortopedista;
class Medico;

// Classe Medico
class Medico {
public:
    virtual void atenderPaciente(Paciente& paciente) = 0;
    virtual Paciente escolherPaciente(vector<Paciente>& fila) = 0;
};

// Classe Paciente
class Paciente {
private:
    sem_t semaforoRetiradaSenha;

public:
    float temperatura;
    int batimentoCardiacos;
    string prioridadeAtendimento;
    int id;
    string nome;
    int idade;
    string motivoVisita;
    string tipoPrioridade;
    bool temNecessidadesEspeciais;
    bool mulherComFilhoDeColo;
    int numeroSenha;
    string momentoRetiradaSenha;
    string especialidadeEncaminhamento;
    string diagnostico;

    Paciente();
    void coletarInformacoes();
    void retirarSenha(int numero);
    void imprimirDetalhesSenha();
    void avaliarSinaisVitais();
    void gerarDiagnostico();
};


Paciente::Paciente() {
    sem_init(&semaforoRetiradaSenha, 0, 1);
}

void Paciente::coletarInformacoes() {

    cout << endl << "Coletando informações do paciente " << id << "." << endl;
    

    nome = "Paciente_" + to_string(id);
    idade = rand() % 100; // Idade aleatória entre 0 e 99
    motivoVisita = "Motivo_" + to_string(rand() % 5); // Motivo fictício

    cout << "Nome: " << nome << ", Idade: " << idade << ", Motivo da Visita: " << motivoVisita << endl;

}

void Paciente::retirarSenha(int numero) {
    sem_wait(&semaforoRetiradaSenha); // Espera pelo semáforo

    numeroSenha = numero; // Definindo o número da senha do paciente
    cout << "Paciente " << id << " retirou a senha " << numeroSenha << endl;

    // Capturando o momento atual
    auto now = chrono::system_clock::now();
    auto currentTime = chrono::system_clock::to_time_t(now);
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::tm *localTime = std::localtime(&currentTime);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();
    momentoRetiradaSenha = oss.str();

    sem_post(&semaforoRetiradaSenha); // Libera o semáforo após a operação ser concluída
}

void Paciente::imprimirDetalhesSenha() {
    cout << "Paciente: " << id << ", Prioridade: " << tipoPrioridade << ", Senha: " << numeroSenha;
    if (temNecessidadesEspeciais) {
        cout << ", Necessidades Especiais: Sim";
    }
    if (mulherComFilhoDeColo) {
        cout << ", Mulher com Filho de Colo: Sim";
    }
    cout << ", Momento da retirada: " << momentoRetiradaSenha;
    cout << endl << endl;
}

void Paciente::gerarDiagnostico() {

    if (motivoVisita.find("Motivo_0") != string::npos) {
        diagnostico = "Diagnóstico A";
    } else if (motivoVisita.find("Motivo_1") != string::npos) {
        diagnostico = "Diagnóstico B";
    } else {
        diagnostico = "Diagnóstico C";
    }
    cout << "Paciente " << id << " recebeu o diagnóstico: " << diagnostico << endl;
}

void Paciente::avaliarSinaisVitais() {
    // Simulação da avaliação dos sinais vitais
    temperatura = (rand() % 5) + 36; // Temperatura aleatória entre 36 e 40
    batimentoCardiacos = (rand() % 61) + 60; // Batimentos cardíacos aleatórios entre 60 e 120

    // Decidindo a especialidade de encaminhamento
    int aleatorio = rand() % 2; // Gera 0 ou 1
    especialidadeEncaminhamento = (aleatorio == 0) ? "Clinico" : "Ortopedista";

    // Determinando a prioridade de atendimento
    if (temperatura > 38 || batimentoCardiacos > 100) {
        prioridadeAtendimento = "Emergência";
    } else if (temperatura > 37 || batimentoCardiacos > 80) {
        prioridadeAtendimento = "Grave";
    } else {
        prioridadeAtendimento = "Normal";
    }
}


// Classe Clinico
class Clinico : public Medico {
public:
    void atenderPaciente(Paciente& paciente) override;
    Paciente escolherPaciente(vector<Paciente>& fila) override;
};

// Classe Ortopedista
class Ortopedista : public Medico {
public:
    void atenderPaciente(Paciente& paciente) override;
    Paciente escolherPaciente(vector<Paciente>& fila) override;
};

// Implementação para Clinico
void Clinico::atenderPaciente(Paciente& paciente) {
    // Simulando o atendimento do paciente pelo clínico
    cout << "Clínico atendendo paciente " << paciente.id << " - " << paciente.nome << endl;
}

Paciente Clinico::escolherPaciente(vector<Paciente>& fila) {
    // Escolhendo o paciente a ser atendido
    if (!fila.empty()) {
        // Suponha que escolhemos o primeiro paciente da fila
        Paciente pacienteEscolhido = fila.front();
        fila.erase(fila.begin());
        return pacienteEscolhido;
    }
    return Paciente(); // Retornar um paciente vazio se a fila estiver vazia
}

// Implementação para Ortopedista
void Ortopedista::atenderPaciente(Paciente& paciente) {
    // Simulando o atendimento do paciente pelo ortopedista
    cout << "Ortopedista atendendo paciente " << paciente.id << " - " << paciente.nome << endl;
    // Aqui você pode adicionar mais lógica de atendimento
}
Paciente Ortopedista::escolherPaciente(vector<Paciente>& fila) {
    // Escolhendo o paciente a ser atendido
    if (!fila.empty()) {
        // Suponha que escolhemos o primeiro paciente da fila
        Paciente pacienteEscolhido = fila.front();
        fila.erase(fila.begin());
        return pacienteEscolhido;
    }
    return Paciente(); // Retornar um paciente vazio se a fila estiver vazia
}

// Classe Hospital
class Hospital {
private:
    vector<Paciente> filaClinicos;
    vector<Paciente> filaOrtopedistas;
    mutex mtxClinicos, mtxOrtopedistas;

    void classificarFila(vector<Paciente>& fila) {
        sort(fila.begin(), fila.end(), [](const Paciente& a, const Paciente& b) {
            return a.prioridadeAtendimento > b.prioridadeAtendimento;
        });
    }

public:
    void adicionarPacienteClinico(const Paciente& paciente);
    void adicionarPacienteOrtopedista(const Paciente& paciente);
    Paciente getProximoPacienteClinico();
    Paciente getProximoPacienteOrtopedista();
    bool filaClinicosVazia() const;
    bool filaOrtopedistasVazia() const;
    void imprimirEstadoFilas() const;
};

// Implementações de Hospital
void Hospital::adicionarPacienteClinico(const Paciente& paciente) {
    lock_guard<mutex> guard(mtxClinicos);
    filaClinicos.push_back(paciente);
    classificarFila(filaClinicos);
}

void Hospital::adicionarPacienteOrtopedista(const Paciente& paciente) {
    lock_guard<mutex> guard(mtxOrtopedistas);
    filaOrtopedistas.push_back(paciente);
    classificarFila(filaOrtopedistas);
}

Paciente Hospital::getProximoPacienteClinico() {
    lock_guard<mutex> guard(mtxClinicos);
    if (filaClinicos.empty()) {
        throw runtime_error("Fila de Clínicos vazia.");
    }
    Paciente paciente = filaClinicos.front();
    filaClinicos.erase(filaClinicos.begin());
    return paciente;
}

Paciente Hospital::getProximoPacienteOrtopedista() {
    lock_guard<mutex> guard(mtxOrtopedistas);
    if (filaOrtopedistas.empty()) {
        throw runtime_error("Fila de Ortopedistas vazia.");
    }
    Paciente paciente = filaOrtopedistas.front();
    filaOrtopedistas.erase(filaOrtopedistas.begin());
    return paciente;
}

bool Hospital::filaClinicosVazia() const {
    return filaClinicos.empty();
}

bool Hospital::filaOrtopedistasVazia() const {
    return filaOrtopedistas.empty();
}

void Hospital::imprimirEstadoFilas() const {
    cout << "Fila de Clínicos: " << filaClinicos.size() << " pacientes\n";
    cout << "Fila de Ortopedistas: " << filaOrtopedistas.size() << " pacientes\n";
}


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

class SalaEspera3 {
private:
    vector<Paciente> pacientesEsperando;
    mutex mtx; // Mutex para sincronização
public:
    const int maxSize = 80;

    // Adicionar um paciente à sala de espera
    bool adicionarPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);
        if (pacientesEsperando.size() >= maxSize) {
            cout << "Sala de Espera 3 está cheia. Aguardando vaga para o paciente " << paciente.id << "." << endl;
            return false;
        }
        pacientesEsperando.push_back(paciente);
        cout << "Paciente " << paciente.id << " adicionado à Sala de Espera 3." << endl;
        return true;
    }

    // Remover um paciente da sala de espera
    void removerPaciente(const Paciente& paciente) {
        lock_guard<mutex> guard(mtx);
        auto it = remove_if(pacientesEsperando.begin(), pacientesEsperando.end(), 
                            [&paciente](const Paciente& p) { return p.id == paciente.id; });
        if (it != pacientesEsperando.end()) {
            pacientesEsperando.erase(it, pacientesEsperando.end());
            cout << "Paciente " << paciente.id << " removido da Sala de Espera 3." << endl;
        } else {
            cout << "Paciente " << paciente.id << " não encontrado na Sala de Espera 3." << endl;
        }
    }

    // Verificar se a sala está cheia
    bool salaCheia() {
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.size() >= maxSize;
    }

    // Verificar se a sala está vazia
    bool salaVazia() {
        lock_guard<mutex> guard(mtx);
        return pacientesEsperando.empty();
    }

    // Imprimir a quantidade de pacientes na sala
    void imprimirQuantidadePacientes() {
        lock_guard<mutex> guard(mtx);
        cout << "Quantidade de pacientes na Sala de Espera 3: " << pacientesEsperando.size() << endl;
    }

    // Obter o próximo paciente para atendimento
    Paciente getProximoPaciente() {
        lock_guard<mutex> guard(mtx);
        if (pacientesEsperando.empty()) {
            throw std::runtime_error("Sala de espera 3 vazia.");
        }
        Paciente paciente = pacientesEsperando.front();
        pacientesEsperando.erase(pacientesEsperando.begin());
        return paciente;
    }

   
};

class Enfermeira {
private:
    atomic<bool> finalizarThread{false};
    atomic<bool>& notificacao;
    bool atenderPorClinico = true; // Novo sinalizador para alternar entre clínico e ortopedista

public:
    Enfermeira(atomic<bool>& notif) : notificacao(notif) {}

    void chamarPacienteDaTriagem(SalaEspera2& salaEspera2, SalaEspera3& salaEspera3) {
        Clinico clinico;
        Ortopedista ortopedista;
        Hospital hospital;

        while (!finalizarThread || !salaEspera2.salaVazia()) {
        if (!salaEspera2.salaVazia()) {
            try {
                Paciente paciente = salaEspera2.getPrioridade();
                salaEspera2.removerPaciente(paciente);

                cout << "Enfermeira iniciando triagem do paciente ID: " << paciente.id << endl;
                // Triagem
                paciente.avaliarSinaisVitais();
                // Encaminhar para clínico ou ortopedista
                if (paciente.especialidadeEncaminhamento == "Clinico") {
                hospital.adicionarPacienteClinico(paciente);
                cout << "Paciente " << paciente.id << " adicionado à fila do Clínico" << endl;
            } else {
                hospital.adicionarPacienteOrtopedista(paciente);
                cout << "Paciente " << paciente.id << " adicionado à fila do Ortopedista" << endl;
            }
                // Aguardar até que haja espaço na SalaEspera3
                while (salaEspera3.salaCheia()) {
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
                salaEspera3.adicionarPaciente(paciente);
            } catch (const std::runtime_error& e) {
                cout << e.what() << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        }}

    
    void finalizar() {
        finalizarThread = true;
        notificacao = true;
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