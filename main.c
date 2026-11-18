
#include <stdio.h>
#include <stdlib.h>
#include "meuBanco.h"

int main() {

    int opcao;

    do {

        printf("\n========== CARTEIRA DIGITAL ==========\n");

        printf("\n1 - Cadastrar cliente");
        printf("\n2 - Abrir conta");
        printf("\n3 - Listar clientes");
        printf("\n4 - Consultar saldo e historico");
        printf("\n5 - Depositar");
        printf("\n6 - Sacar");
        printf("\n7 - Solicitar emprestimo");
        printf("\n0 - Sair");

        printf("\n\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                cadastrarCliente();
                break;

            case 2:
                abrirConta();
                break;

            case 3:
                listarClientes();
                break;

            case 4:
                consultarSaldo();
                break;

            case 5:
                depositar();
                break;

            case 6:
                sacar();
                break;

            case 7:
                solicitarEmprestimo();
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                liberarMemoria();
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
