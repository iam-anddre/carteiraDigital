
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meuBanco.h"

Cliente *clientes = NULL;

int quantidadeClientes = 0;
int proximoId = 1;
int sequencialConta = 1;

//CADASTRAR CLIENTE
void cadastrarCliente() {

    Cliente *temp = realloc(
        clientes,
        (quantidadeClientes + 1) * sizeof(Cliente)
    );

    if (temp == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    clientes = temp;

    Cliente *novo = &clientes[quantidadeClientes];

    novo->id = proximoId++;

    printf("Nome do cliente: ");
    scanf(" %99[^\n]", novo->nome);

    novo->contas = NULL;
    novo->quantidadeContas = 0;

    quantidadeClientes++;

    printf("\nCliente cadastrado!\n");
    printf("ID: %d\n", novo->id);
}

//ABRIR CONTA
void abrirConta() {

    int id, tipo;
    int encontrado = -1;

    printf("Digite o ID do cliente: ");
    scanf("%d", &id);

    for (int i = 0; i < quantidadeClientes; i++) {

        if (clientes[i].id == id) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    printf("\n1 - Corrente");
    printf("\n2 - Salario");
    printf("\n3 - Poupanca");
    printf("\nEscolha o tipo: ");

    scanf("%d", &tipo);

    if (tipo < 1 || tipo > 3) {
        printf("Tipo invalido!\n");
        return;
    }

    Cliente *cliente = &clientes[encontrado];

    Conta *temp = realloc(
        cliente->contas,
        (cliente->quantidadeContas + 1) * sizeof(Conta)
    );

    if (temp == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    cliente->contas = temp;

    Conta *nova = &cliente->contas[cliente->quantidadeContas];

    int base = 1000 + (sequencialConta - 1) / 99;
    int codigo = (sequencialConta - 1) % 99 + 1;

    snprintf(nova->numero, sizeof(nova->numero),
             "%d-%02d", base, codigo);

    sequencialConta++;

    nova->tipo = tipo;
    nova->saldo = 0;

    strcpy(nova->historico, "Conta criada.\n");

    cliente->quantidadeContas++;

    printf("\nConta criada com sucesso!\n");
    printf("Numero: %s\n", nova->numero);
}

//LISTAR CLIENTES
void listarClientes() {

    if (quantidadeClientes == 0) {
        printf("Nenhum cliente cadastrado!\n");
        return;
    }

    for (int i = 0; i < quantidadeClientes; i++) {

        printf("ID: %d\n", clientes[i].id);
        printf("Nome: %s\n", clientes[i].nome);

        for (int j = 0;
             j < clientes[i].quantidadeContas; j++) {

            Conta *conta = &clientes[i].contas[j];

            printf("\nConta: %s\n", conta->numero);

            if (conta->tipo == 1)
                printf("Tipo: Corrente\n");

            else if (conta->tipo == 2)
                printf("Tipo: Salario\n");

            else
                printf("Tipo: Poupanca\n");

            printf("Saldo: R$ %.2f\n", conta->saldo);
        }
    }
}

//CONSULTAR SALDO
void consultarSaldo() {
    int id, tipo;
    int encontrado = -1;

    printf("Digite o ID do cliente: ");
    scanf("%d", &id);

    printf("Digite o tipo da conta (1 - Corrente, 2 - Salario, 3 - Poupanca): ");
    scanf("%d", &tipo);

    if (tipo < 1 || tipo > 3) {
        printf("Tipo invalido!\n");
        return;
    }

    for (int i = 0; i < quantidadeClientes; i++) {
        if (clientes[i].id == id) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    Cliente *cliente = &clientes[encontrado];

    for (int j = 0; j < cliente->quantidadeContas; j++) {
        Conta *conta = &cliente->contas[j];

        if (conta->tipo == tipo) {
            printf("Saldo: R$ %.2f\n", conta->saldo);
            return;
        }
    }

    printf("Conta nao encontrada!\n");
}

//DEPOSITAR
void depositar() {}

//SACAR
void sacar() {}

//SOLICITAR EMPRESTIMO
void solicitarEmprestimo() {}

//LIBERAR MEMORIA
void liberarMemoria(void) {

    for (int i = 0; i < quantidadeClientes; i++) {
        free(clientes[i].contas);
    }

    free(clientes);
}
