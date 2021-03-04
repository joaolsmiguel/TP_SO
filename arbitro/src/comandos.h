/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../../include/estruturas.h"

extern JOGADORES players[MAXPLAYER];
extern JOGO_STRUCT jogo;

extern int jogadores_ativos;
extern int TEMPO;
char bloq_comms[30][20];
extern int em_campeonato;
int n_bloq_comms;

extern int print_active_players();
extern void verifica_jogos_existentes();
extern int verifica_nome_existe(char*);
extern void envia_para_cliente(int, JOGADORES);   //jogador - struck com flag
extern int acaba_campeonato();

void verifica_cmd(char* );
void get_nome(char*, char*);//in, out

void printf_bloq();
extern void apaga_user(int);

int bloqueia_comms_jogador(char*);      //1 bloqueou    0 deu erro
int verifica_jogador_bloqueado(char*);  //-1 se nao esta
int desbloqueia_comms_jogador(char*);   //1 desbloqueou 0 erro