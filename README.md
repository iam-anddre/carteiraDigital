# Carteira Digital

Programa de terminal escrito em C para simular operações básicas de um banco. O sistema permite cadastrar clientes, criar contas e movimentar seus saldos durante a execução.

## Funcionalidades

- Abrir o banco antes de usar as operações.
- Cadastrar e listar clientes.
- Abrir contas Corrente, Salário ou Poupança para clientes.
- Consultar o saldo e o histórico de uma conta.
- Depositar e sacar valores.
- Solicitar empréstimo, sujeito às regras implementadas no programa.
- Consultar o histórico geral do banco, que exibe o saldo total e os clientes aptos a empréstimo.
- Sortear um cliente cadastrado.

## Arquivos

- `main.c` — exibe o menu e encaminha as opções escolhidas.
- `banco.c` — implementa o cadastro de clientes, contas, movimentações, empréstimos e demais operações.
- `meuBanco.h` — define as estruturas e declara as funções compartilhadas.

## Requisitos

É necessário ter um compilador C instalado, como o GCC ou o Clang.
