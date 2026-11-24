#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meuBanco.h"

Cliente *clientes = NULL;
int quantidadeClientes = 0, proximoId = 1, sequencialConta = 1;
int bancoAberto = 0;
const char *TIPOS[] = {"", "Corrente", "Salario", "Poupanca"};

// Le um valor com scanf; retorna 1 se leu, 0 se a entrada foi invalida
int ler(const char *msg, const char *formato, void *destino)
{
    printf("%s", msg);

    int r = scanf(formato, destino);

    if (r != 1)
    {
        printf("Entrada invalida!\n");

        for (int c; (c = getchar()) != '\n' && c != EOF;)
            ;

        return 0;
    }

    return 1;
}

// Acrescenta uma linha ao historico
void registrar(Conta *conta, const char *evento, float valor)
{
    char linha[64];
    snprintf(linha, sizeof linha, "%s: R$ %.2f\n", evento, valor);
    strncat(conta->historico, linha, MAX_HISTORICO - strlen(conta->historico) - 1);
}

// Pede o ID
Cliente *buscarCliente(void)
{
    int id;
    if (!ler("Digite o ID do cliente: ", "%d", &id))
        return NULL;

    for (int i = 0; i < quantidadeClientes; i++)
        if (clientes[i].id == id)
            return &clientes[i];

    printf("Cliente nao encontrado!\n");
    return NULL;
}

Conta *contaDoTipo(Cliente *cliente, int tipo)
{
    for (int j = 0; j < cliente->quantidadeContas; j++)
        if (cliente->contas[j].tipo == tipo)
            return &cliente->contas[j];
    return NULL;
}

Conta *pedirConta(void)
{
    int tipo;
    Cliente *cliente = buscarCliente();

    if (!cliente || !ler("Tipo da conta (1 - Corrente, 2 - Salario, 3 - Poupanca): ", "%d", &tipo))
        return NULL;

    if (tipo < 1 || tipo > 3)
    {
        printf("Tipo invalido!\n");
        return NULL;
    }

    Conta *conta = contaDoTipo(cliente, tipo);
    if (!conta)
        printf("Conta nao encontrada!\n");
    return conta;
}
Conta *aptoEmprestimo(Cliente *cliente, float *limite)
{
    float total = 0;
    int temPoupanca = 0;

    for (int j = 0; j < cliente->quantidadeContas; j++)
    {
        total += cliente->contas[j].saldo;
        if (cliente->contas[j].tipo == 3 && cliente->contas[j].saldo > 0)
            temPoupanca = 1;
    }

    *limite = total * 0.25f * 10;
    Conta *corrente = contaDoTipo(cliente, 1);
    return (!cliente->possuiEmprestimo && corrente && temPoupanca) ? corrente : NULL;
}

void cadastrarCliente(void)
{
    Cliente *temp = realloc(clientes, (quantidadeClientes + 1) * sizeof(Cliente));
    if (!temp)
    {
        printf("Erro de memoria!\n");
        return;
    }
    clientes = temp;

    Cliente *novo = &clientes[quantidadeClientes];
    *novo = (Cliente){.id = proximoId}; // zera contas, emprestimo, etc.

    printf("Nome do cliente: ");
    if (scanf(" %99[^\n]", novo->nome) != 1)
        return;

    proximoId++;
    quantidadeClientes++;
    printf("\nCliente cadastrado!\nID: %d\n", novo->id);
}

void abrirConta(void)
{
    int tipo;
    Cliente *cliente = buscarCliente();

    if (!cliente || !ler("\n1 - Corrente\n2 - Salario\n3 - Poupanca\nEscolha o tipo: ", "%d", &tipo))
        return;

    if (tipo < 1 || tipo > 3)
    {
        printf("Tipo invalido!\n");
        return;
    }
    if (contaDoTipo(cliente, tipo))
    {
        printf("O cliente ja possui uma conta desse tipo!\n");
        return;
    }

    Conta *temp = realloc(cliente->contas, (cliente->quantidadeContas + 1) * sizeof(Conta));
    if (!temp)
    {
        printf("Erro de memoria!\n");
        return;
    }
    cliente->contas = temp;

    Conta *nova = &cliente->contas[cliente->quantidadeContas++];
    *nova = (Conta){.tipo = tipo};
    snprintf(nova->numero, sizeof nova->numero, "%d-%02d",
             1000 + (sequencialConta - 1) / 99, (sequencialConta - 1) % 99 + 1);
    sequencialConta++;
    strcpy(nova->historico, "Conta criada.\n");

    printf("\nConta criada com sucesso!\nNumero: %s\n", nova->numero);
}

void listarClientes(void)
{
    if (!quantidadeClientes)
        printf("Nenhum cliente cadastrado!\n");

    for (int i = 0; i < quantidadeClientes; i++)
    {
        printf("ID: %d\nNome: %s\n", clientes[i].id, clientes[i].nome);

        for (int j = 0; j < clientes[i].quantidadeContas; j++)
        {
            Conta *conta = &clientes[i].contas[j];
            printf("\nConta: %s\nTipo: %s\nSaldo: R$ %.2f\n", conta->numero, TIPOS[conta->tipo], conta->saldo);
        }
    }
}

void consultarSaldo(void)
{
    Conta *conta = pedirConta();
    if (conta)
        printf("Conta: %s\nSaldo: R$ %.2f\nHistorico:\n%s", conta->numero, conta->saldo, conta->historico);
}

// Deposito a
void movimentar(int saque)
{
    float valor;
    Conta *conta = pedirConta();

    if (!conta || !ler(saque ? "Qual o valor para sacar? " : "Qual o valor a ser depositado? ", "%f", &valor))
        return;

    if (valor <= 0)
    {
        printf("Insira um valor valido!\n");
        return;
    }
    if (saque && valor > conta->saldo)
    {
        printf("Saldo insuficiente!\n");
        return;
    }

    conta->saldo += saque ? -valor : valor;
    registrar(conta, saque ? "Saque" : "Deposito", valor);
    printf("%s realizado com sucesso!\nNovo saldo: R$ %.2f\n", saque ? "Saque" : "Deposito", conta->saldo);
}

void depositar(void) { movimentar(0); }
void sacar(void) { movimentar(1); }

void solicitarEmprestimo(void)
{
    float limite, valor;
    Cliente *cliente = buscarCliente();
    if (!cliente)
        return;

    Conta *corrente = aptoEmprestimo(cliente, &limite);
    if (!corrente)
    {
        printf("O cliente nao pode solicitar emprestimo!\n");
        return;
    }

    printf("Limite maximo de emprestimo: R$ %.2f\n", limite);
    if (!ler("Qual o valor que voce deseja solicitar? ", "%f", &valor))
        return;

    if (valor <= 0 || valor > limite)
    {
        printf("Emprestimo invalido!\n");
        return;
    }

    corrente->saldo += valor;
    cliente->possuiEmprestimo = 1;
    cliente->valorEmprestimo = valor;
    registrar(corrente, "Emprestimo", valor);

    printf("Emprestimo aprovado!\nValor liberado: R$ %.2f\nNovo saldo da conta corrente: R$ %.2f\n",
           valor, corrente->saldo);
}

// HISTORICO DO BANCO:
void historico(void)
{
    float total = 0, limite;

    for (int i = 0; i < quantidadeClientes; i++)
        for (int j = 0; j < clientes[i].quantidadeContas; j++)
            total += clientes[i].contas[j].saldo;

    printf("\n== Saldo Total do banco ==\n%d | R$ %.2f\n", quantidadeClientes, total);
    printf("\n== Clientes Aptos a Empréstimos ==\n");

    for (int i = 0; i < quantidadeClientes; i++)
    {
        Conta *corrente = aptoEmprestimo(&clientes[i], &limite);
        if (corrente)
            printf("%s|%s|%.2f\n", clientes[i].nome, corrente->numero, limite);
    }
}

void sortearCliente(void)
{
    if (!quantidadeClientes)
        printf("\nNao existem clientes cadastrados!\n");
    else
        printf("\nCliente sorteado: %s\n", clientes[rand() % quantidadeClientes].nome);
}

// Fibonacci recursivo: 0, 1, 1, 2, 3, 5...
static int fibonacci(int n)
{
    return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

static float saldoTotal(Cliente *cliente)
{
    float total = 0;
    for (int j = 0; j < cliente->quantidadeContas; j++)
        total += cliente->contas[j].saldo;
    return total;
}

void comprarTitulo(void)
{
    Cliente *cliente = buscarCliente();
    if (!cliente)
        return;

    if (cliente->possuiTitulo)
        printf("O cliente ja possui titulo de capitalizacao!\n");
    else
    {
        cliente->possuiTitulo = 1;
        printf("Titulo de capitalizacao adquirido por %s!\n", cliente->nome);
    }
}

// SORTEIO: 5 maiores saldos, todos precisam ter titulo.
// Peso de cada um = Fibonacci: 1o lugar = 5, 2o = 3, 3o = 2, 4o = 1, 5o = 1 (total 12)
void sortearCliente(void)
{
    if (quantidadeClientes < 5)
    {
        printf("\nSao necessarios pelo menos 5 clientes para o sorteio!\n");
        return;
    }

    Cliente *top[5];
    int usado[quantidadeClientes];
    memset(usado, 0, sizeof usado);

    for (int k = 0; k < 5; k++) // escolhe o maior saldo ainda nao escolhido
    {
        int melhor = -1;
        for (int i = 0; i < quantidadeClientes; i++)
            if (!usado[i] && (melhor < 0 || saldoTotal(&clientes[i]) > saldoTotal(&clientes[melhor])))
                melhor = i;
        usado[melhor] = 1;
        top[k] = &clientes[melhor];
    }

    int semTitulo = 0;
    printf("\n== 5 maiores saldos ==\n");
    for (int k = 0; k < 5; k++)
    {
        printf("%do) %s | R$ %.2f | Titulo: %s | Peso: %d\n", k + 1, top[k]->nome,
               saldoTotal(top[k]), top[k]->possuiTitulo ? "Sim" : "Nao", fibonacci(5 - k));
        if (!top[k]->possuiTitulo)
            semTitulo = 1;
    }

    if (semTitulo)
    {
        printf("\nSorteio cancelado: todos os participantes precisam ter titulo de capitalizacao!\n");
        return;
    }

    int somaPesos = 0;
    for (int k = 0; k < 5; k++)
        somaPesos += fibonacci(5 - k);

    int numero = rand() % somaPesos, acumulado = 0;
    for (int k = 0; k < 5; k++)
    {
        acumulado += fibonacci(5 - k);
        if (numero < acumulado)
        {
            printf("\nNumero sorteado: %d de %d\nCliente sorteado: %s\n", numero + 1, somaPesos, top[k]->nome);
            return;
        }
    }
}


void liberarMemoria(void)
{
    for (int i = 0; i < quantidadeClientes; i++)
        free(clientes[i].contas);
    free(clientes);
    clientes = NULL;
    quantidadeClientes = 0;
}
