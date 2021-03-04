/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "g_milionario.h"

int pontos = 0;

void trata_sinal(int sig){
    if (sig == SIGUSR1){
        //printf("\nPontos: %i\n",pontos);
        exit(pontos);
    }
}

int ver_qnt_nos(pergunta_resposta* p){
    int i = 0;
    while (p != NULL){
        p = p->prox;
        i++;
    }
    return i;
}

pergunta_resposta* ler_ficheiro(char* caminho){
    FILE *f;

    pergunta_resposta *novo, *aux;
    pergunta_resposta* p = NULL;

    f = fopen(caminho, "r");
    if (f == NULL){ perror("fopen"); return NULL; }

    while (!feof(f)){
        novo = malloc(sizeof(pergunta_resposta));
        if (novo == NULL){
            free(novo);
        }

        fscanf(f, "%i\n%[^\n]", &novo->id_pergunta, novo->pergunta);
        for (int i = 0; i < 2; i++){
            fscanf(f, "\n%[^\n]", novo->respostas[i]);
        }
        fscanf(f, "\n%i", &novo->resposta_certa);
        novo->prox = NULL;

        if (p == NULL){
            p = novo;
        }else{
            aux = p;
            while (aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }
    fclose(f);
    return p;
}


int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    signal(SIGUSR1, trata_sinal);

    srand ( time(NULL) );

    pergunta_resposta* array_perguntas = ler_ficheiro("../jogos/g_files/qqsm.txt");

    if (array_perguntas == NULL){
        printf("\nocurreu um erro!\n");
        return 0;
    }
    
    while (1)
    {
        printf("\nJogo: Quem Quer ser Milionario!!\n");
        int n = rand() % ver_qnt_nos(array_perguntas) + 1;

        pergunta_resposta* aux = array_perguntas;
        while (aux->id_pergunta != n){
            aux = aux->prox;
        }

        printf("\nPergunta %i: %s", aux->id_pergunta, aux->pergunta);
        for (int i = 0; i < 2; i++){
            printf("\nResposta %i:\t%s", i+1 , aux->respostas[i]);
        }

        printf("\n\nResposta certa (Digite o numero da resposta):");
        
        char op[5];
        fflush(stdout);
        scanf("%s", op);

        if ( atoi(op) == aux->resposta_certa ){
            printf("acertou");
            pontos = pontos + 5;
        }else{
            printf("erro");
        }
    }
}