/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "../include/estruturas.h"

int pontos = 0;

void trata_sinal(int sig){
    if (sig == SIGUSR1){
        //printf("\nPontos: %i\n",pontos);
        exit(pontos);
    }
}

int main() {
    setbuf(stdout, NULL);
    srand ( time(NULL) );
    signal(SIGUSR1, trata_sinal);

    while(1){
        int n;
        char op[5];
        
        printf("Escolha um numero entre 0-10:");
        fflush(stdout);
        scanf("%s", op);


        n = rand() % 10 + 1;
        if( atoi(op) == n ){
            printf("Acertou!!!\n");
            pontos = pontos + 5;
        }else{
            printf("Errou :c \n");
        }
    }
}