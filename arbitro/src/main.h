/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../../include/estruturas.h"
#include "configs.h"

extern JOGO_STRUCT jogo;
extern JOGADORES players[MAXPLAYER];
extern int jogadores_ativos;
extern int TEMPO;

extern int n_bloq_comms;

extern void* arranca_jogo_e_redireciona(int);
extern void verifica_cmd(char* );

int main(int, char**);
void verifica_var_env();