/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../include/estruturas.h"


typedef struct pergunta pergunta_resposta, *ppergunta_resposta;
struct pergunta{
    int id_pergunta;
    char pergunta[100];
    char respostas[2][50];
    int resposta_certa;
    pergunta_resposta* prox;
};


int main(int, char**);
pergunta_resposta* ler_ficheiro(char*);
int ver_qnt_nos(pergunta_resposta*);
void trata_sinal(int);