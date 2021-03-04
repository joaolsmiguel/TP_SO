/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "main.h"

pthread_t array_thr[MAXPLAYER];

JOGO_STRUCT jogo;
JOGADORES players[MAXPLAYER];
int jogadores_ativos = 0;
int TEMPO = 0;

void verifica_var_env(){
    jogo.pid_server = getpid();
    
    //Verificar var env GAMEDIR
    if (getenv("GAMEDIR") == NULL){
        strcpy(jogo.dir_jogos, GAMEDIR);
    }else{
        strcpy(jogo.dir_jogos, getenv("GAMEDIR"));
    }

    //Verificar var env MAXPLAYER
    if (getenv("MAXPLAYER") == NULL){
        jogo.max_player = MAXPLAYER;
    }else{
        jogo.max_player = atoi(getenv("MAXPLAYER"));

        if (jogo.max_player > MAXPLAYER){
            jogo.max_player = MAXPLAYER;
        }
    }

    //verificar timeout e duracao_campeonato
    if (jogo.timeout == 0){
        printf("Defina o TIMEOUT: ");
        scanf("%i", &jogo.timeout);
    }
    if(jogo.duracao_campeonato == 0){
        printf("Defina a DURACAO DO CAMPEONATO: ");
        scanf("%i", &jogo.duracao_campeonato);
    }
}

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    srand ( time(NULL) );

    memset(&jogo, 0, sizeof(JOGO_STRUCT));
    memset(&players, 0, sizeof(JOGADORES) * jogo.max_player);

    pthread_mutex_t lock1, lock2;
    pthread_t thread_comunica, thread_relogio;

    signal(SIGINT, interpreta_sinal);
    signal(SIGUSR1, interpreta_sinal);
    signal(SIGUSR2, interpreta_sinal);

    int opt;
    while ((opt = getopt (argc, argv, "t:d:")) != -1){  //optarg var que tem os inputs
        switch (opt){
            case 't':
                jogo.timeout = atoi(optarg);
                break;
            
            case 'd':
                jogo.duracao_campeonato = atoi(optarg);
                break;

            default:
                break;
        }
    }
    system("clear");

    verifica_var_env();
    cria_pipe();

    if (pthread_mutex_init(&lock1, NULL) != 0){          //cria um auxiliar para parar o thread e nao continuar
        printf("\nERRO a criar o lock\n");
        exit(0);
    }

    if (pthread_mutex_init(&lock2, NULL) != 0){
        printf("\nERRO a criar o lock\n");
        exit(0);
    }

    if (pthread_create(&thread_relogio, NULL, (void*) &relogio, &lock2) != 0){
        printf("\nErro a criar thread\n");
        exit(0);
    }

    if (pthread_create(&thread_comunica, NULL, (void*)&recebe_info, &lock1) != 0){
        printf("\nErro a criar thread\n");
        exit(0);
    }
    
    n_bloq_comms = 0;
    char comando[50];

    while (1){
        printf("Comando: ");
        scanf(" %49[^\n]s", comando);        
        
        verifica_cmd(comando);
    }

    pthread_join(thread_comunica, NULL);
    pthread_join(thread_relogio, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    
    fecha_server();

    return EXIT_SUCCESS;
}