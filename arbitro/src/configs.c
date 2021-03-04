/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "configs.h"

char g_array[][30] = {G_01, G_02, G_03};

int verifica_nome_existe(char* nome){
    for (int i = 0; i < jogadores_ativos; i++){
        if (strcmp(players[i].nome, nome) == 0){
            return i;
        }
    }
    return -1;
}

void interpreta_sinal(int sig){
    if(sig == SIGINT){
        fecha_server();
    }
}

void cria_pipe(){
    if (access(PIPE_DIR, S_IRWXU) == 0){
        exit(0);
    }

    if (mkfifo(PIPE_DIR, S_IRWXU) != 0){
        exit(0);
    }
}

void print_active_players(){
    if (jogadores_ativos > 0){
        printf("\njogadores ativos: %i", jogadores_ativos);
        for (int i = 0; i < jogadores_ativos; i++){
            printf("\nnome jogador: %s\n", players[i].nome);
        }
    }else{
        printf("\nNao existem jogadores ativos\n");
    }
}

void update_info_jogadores(JOGADORES recebe, int n_jogador){
    //copia info_player
    players[n_jogador].pid_jogador = recebe.pid_jogador;
    strcpy(players[n_jogador].jogando, recebe.jogando);
    strcpy(players[n_jogador].nome, recebe.nome);
}

void apaga_user(int pid_a_remover){
    if (jogadores_ativos == jogo.max_player){
        JOGADORES tmp;
        players[jogo.max_player] = tmp;
    }else{
        for (int i = 0; i < jogadores_ativos; i++){
            if (players[i].pid_jogador == pid_a_remover){
                players[i] = players[i + 1];
                if (strcmp(players[i].nome, players[i+1].nome) == 0){
                    players[i] = players[i + 2];
                }

                if (i <= 0){
                    i = 0;
                }else{
                    i--;
                }
            }
        }
    }
    jogadores_ativos--;

    if (jogadores_ativos < 2){
        acaba_campeonato();
    }
}


void* recebe_info(pthread_mutex_t *lock){
    char pipe_cliente[25];
    JOGADORES recebe, envia;
    int fd_recebe, fd_envia;// recebe pipe_server   /      envia pipe_cliente
    
    fd_recebe = open(PIPE_DIR, O_RDWR);

    while (TRUE){
        memset(&recebe, 0, sizeof(JOGADORES));
        memset(&envia, 0, sizeof(JOGADORES));

        read(fd_recebe, &recebe, sizeof(recebe));
        //printf("\depois read - action = %i\n", recebe.action);
        
        if(verifica_jogador_bloqueado(recebe.nome) == -1){

            sprintf(pipe_cliente, "%s-%d", PIPE_DIR, recebe.pid_jogador);

            fd_envia = open(pipe_cliente, O_RDWR);

            if (em_campeonato == 0){
                if (recebe.action == LOG_IN){
                    if (verifica_nome_existe(recebe.nome) == -1){
                        jogadores_ativos++;
                        pthread_mutex_lock(lock);

                        update_info_jogadores(recebe, jogadores_ativos-1);
                        envia.action = ENTROU;
                        write(fd_envia, &envia, sizeof(envia));

                        pthread_mutex_unlock(lock);
                    }else{
                        envia.action = NOME_JA_EXISTE;
                        write(fd_envia, &envia, sizeof(envia));
                    }
                }
            }else if(em_campeonato == 1){
                if (recebe.action == RESPOSTA_PARA_JOGO){
                    pthread_mutex_lock(lock);
                    strcat(recebe.resposta_ao_jogo, "\n");
                    write(array_envia_para_jogo[verifica_nome_existe(recebe.nome)], recebe.resposta_ao_jogo, strlen(recebe.resposta_ao_jogo));

                    pthread_mutex_unlock(lock);
                }else if (recebe.action == LOG_IN){
                    envia.action = SAIU;
                    write(fd_envia, &envia, sizeof(envia));
                }
            }
            
            if (recebe.action == SAIR){
                envia.action = SAIU;
                pthread_mutex_lock(lock);
                write(fd_envia, &envia, sizeof(envia));
                apaga_user(recebe.pid_jogador);
                close(fd_envia);
                pthread_mutex_unlock(lock);
            }else if (recebe.action == DESISTIU){
                pthread_mutex_lock(lock);
                apaga_user(recebe.pid_jogador);
                close(fd_envia);
                pthread_mutex_unlock(lock);
            }
        }
    }
}

int acaba_campeonato(){
    TEMPO = jogo.duracao_campeonato - 1;
    for (int i = 0; i < jogadores_ativos; i++){
        kill(players[i].pid_jogo, SIGUSR1);
        kill(players[i].pid_jogador, SIGUSR1);
    }
}


void* arranca_jogo_e_redireciona(int pos_jogador){

    int fd_interno_stdout[2];       //pipe out
    int fd_interno_stdin[2];        //pipe in
    
    if (pipe(fd_interno_stdout) == -1){
        perror("pipe: ");
        exit(0);
    }

    if (pipe(fd_interno_stdin) == -1){
        perror("pipe: ");
        exit(0);
    }

    array_envia_para_jogo[pos_jogador] = fd_interno_stdin[1];

    int rand1 = rand() % 3;

    //processo filho onde corre o jogo
    players[pos_jogador].pid_jogo = fork();

    if (players[pos_jogador].pid_jogo == 0){        //filho (onde esta o jogo)
        
        char caminho_jogo[30];
        char pipe_cliente[30];

        sprintf(caminho_jogo, "%s%s", GAMEDIR, g_array[rand1]);
        players[pos_jogador].action = RECEBE_PONTOS;    //para enviar o nome do jogo
        strcpy(players[pos_jogador].jogando, g_array[rand1]);
        envia_para_cliente(pos_jogador, players[pos_jogador]);

        close(fd_interno_stdin[1]);
        close(fd_interno_stdout[0]);

        close(STDIN_FILENO);
        close(STDOUT_FILENO);

        dup2(fd_interno_stdin[0], STDIN_FILENO);
        dup2(fd_interno_stdout[1], STDOUT_FILENO);
        
        execl(caminho_jogo, caminho_jogo);
        
    }else{
        //fecha pontas dos pipes que nao vao ser usadas
        close(fd_interno_stdin[0]);
        close(fd_interno_stdout[1]);

        while (em_campeonato){
            char buffer[200];
            memset(&buffer, 0, 200);
            read (fd_interno_stdout[0], &buffer, sizeof(buffer));
            strcpy(players[pos_jogador].pergunta_do_jogo, buffer);
            players[pos_jogador].action = PERGUNTA_DO_JOGO;
            envia_para_cliente(pos_jogador, players[pos_jogador]);
        }

        if (em_campeonato == 0){
            int res_pontos;
            waitpid(-1, &res_pontos, WNOHANG);
            if ( WEXITSTATUS(res_pontos) ){
                players[pos_jogador].action = RECEBE_PONTOS;
                players[pos_jogador].pontos = WEXITSTATUS(res_pontos);
                envia_para_cliente(pos_jogador, players[pos_jogador]);
            }
        }
    }
}


void* relogio(pthread_mutex_t *lock){
    em_campeonato = 0;
    int n_thread_campeonato = 0;

    while (TRUE){
        if (jogadores_ativos >= 2){
            sleep(60);          //mudar para 60 sec
            TEMPO++;

            if (TEMPO == jogo.timeout && em_campeonato == 0){
                em_campeonato = 1;
                TEMPO = 0;
                printf("\n\nAcabou tempo de espera...\tCampeonato vai comecar...\n\n");

                memset(&array_envia_para_jogo, 0, sizeof(int) * MAXPLAYER);
                memset(&array_thr, 0, sizeof(pthread_t) * jogadores_ativos);

                //inicializa os threads para os jogadores ativos
                for (int i = 0; i < jogadores_ativos; i++){
                    if (pthread_create(&array_thr[i], NULL, (void*)&arranca_jogo_e_redireciona, i) != 0){
                        printf("erro criar o thread");
                    }
                    n_thread_campeonato = i;
                }
            }

            if(TEMPO == jogo.duracao_campeonato && em_campeonato == 1){
                acaba_campeonato();
                em_campeonato = 0;
                TEMPO = 0;

                sleep(1);

                for (int i = 0; i < n_thread_campeonato; i++){
                    pthread_cancel(array_thr[i]);
                    close(array_envia_para_jogo[i]);
                    pthread_join(array_thr[i], NULL);
                }
                n_thread_campeonato = 0;
                
                printf("\nAcabou campeonato\nOs jogos vao encerrar\n\n");
            }

        }else{
            //enquanto espera por jogadores
            TEMPO = 0;
            em_campeonato = 0;
        }
    }
}

void fecha_server(){
    JOGADORES envia;
    char pipes[30];

    envia.action = SAIR_SERVIDOR;

    for (int i = 0; i < jogadores_ativos; i++){
        sprintf(pipes, "%s-%i", PIPE_DIR, players[i].pid_jogador);
        int fd = open(pipes, O_WRONLY);
        write(fd, &envia, sizeof(envia));
        close(fd);
    }
    unlink(PIPE_DIR);
    exit(0);
}

void envia_para_cliente(int pos_jogador, JOGADORES envia){
    if (verifica_jogador_bloqueado(envia.nome) == -1){
        char pipe_cliente[30];

        sprintf(pipe_cliente, "%s-%i", PIPE_DIR, players[pos_jogador].pid_jogador);
        int fd_envia = open(pipe_cliente, O_WRONLY);
        
        write(fd_envia, &envia, sizeof(envia));
        close(fd_envia);
    }
}

void verifica_jogos_existentes(){
    char caminho_jogos[50];

    for (int i = 0; i < 3; i++){
        sprintf(caminho_jogos, "%s%s", GAMEDIR, g_array[i]);
        if ( access(caminho_jogos, F_OK) == 0 ){
            printf("\nJogo %s esta disponivel", g_array[i]);
        }else{
            printf("\nJogo %s nao esta disponivel", g_array[i]);
        }
    }
    printf("\n\n");
}