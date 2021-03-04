#include "../include/estruturas.h"

int pontos = 0;

void trata_sinal(int sig){
    if (sig == SIGUSR1){
        //printf("\nPontos: %i\n",pontos);
        exit(pontos);
    }
}

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    signal(SIGUSR1, trata_sinal);
    srand ( time(NULL) );

    char ing[5][10] = { "car", "truck", "fireman", "hello", "mouse" };
    char pt[5][10] = { "carro", "camiao", "bombeiro", "ola", "rato" };

    while (1){
        int random = rand() % 4;
        int novo_random;

        printf("Qual a traducao da seguinte palavra: %s\n",ing[random]);
        printf("\n1 - %s", pt[random]);

        novo_random = random;

        while (novo_random == random){
            novo_random = rand() % 4;
        }
        
        printf("\n2 - %s", pt[novo_random]);

        printf("\nQual e a opcao correta:");

        char op[5];
        scanf("%s", op);
        
        if ( atoi(op) == 1 ){
            printf("\ncorreto\n");
            pontos = pontos + 5;
        }else{
            printf("\nerrado\n");
        }
    }
}