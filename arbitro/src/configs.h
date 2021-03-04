/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../../include/estruturas.h"

pthread_t array_thr[MAXPLAYER];

extern JOGO_STRUCT jogo;
extern JOGADORES players[MAXPLAYER];
extern int jogadores_ativos;
extern int TEMPO;

int em_campeonato;  //0 se nao | 1 se sim

int array_envia_para_jogo[MAXPLAYER];

extern char bloq_comms[30][20];
extern int n_bloq_comms;

void* recebe_info(pthread_mutex_t*);
void* relogio(pthread_mutex_t*);
void* arranca_jogo_e_redireciona(int);  //id_cliente

extern int verifica_nome_existe(char*);

void cria_pipe();//faz verificacao se ja existe um se nao cria

void interpreta_sinal(int);

void update_info_jogadores(JOGADORES, int);

void print_active_players();//print lista jogadores online
void apaga_user(int);

extern void verifica_jogos_existentes();
extern void envia_para_cliente(int, JOGADORES);   //jogador - flag
void fecha_server();

extern int verifica_jogador_bloqueado(char*);

int acaba_campeonato();