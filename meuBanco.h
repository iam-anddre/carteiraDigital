#ifndef MEUBANCO_H
#define MEUBANCO_H

#define MAX_NOME 100
#define MAX_HISTORICO 100

typedef struct {
    char numero[16];
    int codigo;
    int tipo;
    float saldo;
    char historico[MAX_HISTORICO];
} Conta;

typedef struct {
    int id;
    char nome[MAX_NOME];
    Conta *contas;
    int quantidadeContas;
} Cliente;

void cadastrarCliente();
void abrirConta();
void listarClientes();
void consultarSaldo();
void depositar();
void sacar();
void solicitarEmprestimo();
void liberarMemoria(void);

#endif
