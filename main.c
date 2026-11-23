#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "meuBanco.h"
#include "banco.c"

int main(void)
{
    int opcao = 0;
    srand(time(NULL));

    do
    {
        printf("\n========== CARTEIRA DIGITAL ==========\n"
               "\n1 - Abrir banco"
               "\n2 - Cadastrar cliente"
               "\n3 - Abrir conta"
               "\n4 - Listar clientes"
               "\n5 - Consultar saldo e historico da conta"
               "\n6 - Depositar"
               "\n7 - Sacar"
               "\n8 - Solicitar emprestimo"
               "\n9 - Sair"
               "\n10 - Historico do banco"
               "\n11 - Sortear cliente\n");

        if (!ler("\nEscolha uma opcao: ", "%d", &opcao))
            continue;

        if (opcao > 1 && opcao != 9 && !bancoAberto)
        {
            printf("\nAbra o banco primeiro (opcao 1)!\n");
            continue;
        }

        switch (opcao)
        {
        case 1:
            printf(bancoAberto ? "\nO banco ja esta aberto!\n" : "\nBanco aberto com sucesso!\n");
            bancoAberto = 1;
            break;
        case 2:
            cadastrarCliente();
            break;
        case 3:
            abrirConta();
            break;
        case 4:
            listarClientes();
            break;
        case 5:
            consultarSaldo();
            break;
        case 6:
            depositar();
            break;
        case 7:
            sacar();
            break;
        case 8:
            solicitarEmprestimo();
            break;
        case 9:
            printf("\nEncerrando o sistema...\n");
            break;
        case 10:
            historico();
            break;
        case 11:
            sortearCliente();
            break;
        default:
            printf("\nOpcao invalida!\n");
        }
    } while (opcao != 9);

    liberarMemoria();
    return 0;
}
