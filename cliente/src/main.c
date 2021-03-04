/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "main.h"

JOGADORES jogo;
int jogando = 0;

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    int opt;
    pthread_t thread;

    signal(SIGINT, interpreta_sinal);
    signal(SIGUSR1, interpreta_sinal);
    
    system("clear");

    if ((opt = getopt(argc, argv, "u:")) != -1){
        if (optarg){
            strcpy(jogo.nome, optarg);
        }
    }

    if (strcmp(jogo.nome, "") == 0){
        printf("Digite o ser username: ");
        scanf("%s", jogo.nome);
    }

    if ( pthread_create(&thread, NULL, &recebe_info, NULL) != 0 ){
        printf("\n erro ao criar o thread \n");
        exit(0);
    }

    while (TRUE){
        char cmd[50];
        printf(":: ");
        scanf("%s", cmd);

        if (cmd[0] == '#'){
            if (strcmp(cmd, "#mygame") == 0){       //feito pelo server
                //envia_ao_server( DESISTIU );
                if (jogando == 1){
                    int tam_nome_jogo = strlen(jogo.jogando);
                    if (tam_nome_jogo > 0 ){
                        jogo.jogando[ tam_nome_jogo - 2 ] = '\0';
                        printf("\nEsta atualmente a jogar: %s\n", jogo.jogando);
                    }
                }
            }else if(strcmp(cmd, "#quit") == 0){
                jogo.action = DESISTIU;
                envia_ao_server();
                fechar_cliente();
            }else if (strcmp(cmd, "#score") == 0){
                printf("\nVoce tem %i pontos.\n", jogo.pontos);
            }
        }else{
            //envia o input para o servidor
            if (jogando == 1){
                system("clear");
                jogo.action = RESPOSTA_PARA_JOGO;
                strcpy(jogo.resposta_ao_jogo, cmd);

                envia_ao_server();
            }
        }
    }

    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}