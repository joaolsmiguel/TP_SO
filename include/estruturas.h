/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/


#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <limits.h>

/* ----- Variaveis de Ambiente ----- */
#define GAMEDIR "../jogos/"
#define MAXPLAYER 30

#define PIPE_DIR "../PIPE"

#define TRUE 1
#define FALSE 0

//acoes dos clientes
#define LOG_IN 1
#define LOG_OUT 2
#define ENTROU 3
#define SAIR 4
#define SAIU 5
#define SAIR_SERVIDOR 6
#define NOME_JA_EXISTE 7
#define DESISTE 8
#define DESISTIU 9

#define COMECA_JOGO 20
#define PERGUNTA_DO_JOGO 21
#define RESPOSTA_PARA_JOGO 22
#define RECEBE_PONTOS 25

//jogos
#define G_01 "g_rand.o"
#define G_02 "g_milionario.o"
#define G_03 "g_dicionario.o"

typedef struct{
    char nome[20];      //nao pode haver repetido
    pid_t pid_jogador;
    int pontos;
    char jogando[30];
    pid_t pid_jogo;
    char pergunta_do_jogo[200];
    char resposta_ao_jogo[5];
    int action;
} JOGADORES;

typedef struct{
    char dir_jogos[20];
    int max_player;
    int timeout, duracao_campeonato;
    pid_t pid_server;
} JOGO_STRUCT;


#endif