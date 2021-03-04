/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../../include/estruturas.h"

extern JOGADORES jogo;
extern int jogando;

int verifica_conecao(); //Se existir pipe, liga se e envia o fd
int cria_pipe_cliente(char*);    //devolve true se nao houver problemas

void* recebe_info();
void update_info(JOGADORES);
void fechar_cliente();

void interpreta_sinal(int);

void envia_ao_server();