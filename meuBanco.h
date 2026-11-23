#ifndef MEUBANCO_H
#define MEUBANCO_H

#define MAX_NOME 100
#define MAX_HISTORICO 1000

typedef struct
{
    char numero[1];
    int tipo; // Corrente, Salario, Poupanca
    float saldo;
    char historico[MAX_HISTORICO];
} Conta;

typedef struct
{
    int id;
    char nome[MAX_NOME];
    Conta *contas;
    int quantidadeContas;
    int possuiEmprestimo;
    float valorEmprestimo;
} Cliente;

extern int bancoAberto;

int ler(const char *msg, const char *formato, void *destino);
void cadastrarCliente(void);
void abrirConta(void);
void listarClientes(void);
void consultarSaldo(void);
void depositar(void);
void sacar(void);
void solicitarEmprestimo(void);
void historico(void);
void sortearCliente(void);
void liberarMemoria(void);

#endif
