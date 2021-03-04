/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "configs.h"


void interpreta_sinal(int sig){
    if (sig == SIGINT){
        fechar_cliente();
    }else if (sig == SIGUSR1){
        if (jogando == 1){
            jogando = 0;
        }
    }
}

int cria_pipe_cliente(char* cliente_pipe){
    sprintf(cliente_pipe, "%s-%d", PIPE_DIR, getpid());
    
    if (mkfifo(cliente_pipe, S_IRWXU) != 0){
        printf("\nOcurreu um erro ao criar o pipe\n");
        return FALSE;
    }

    int fd = open(cliente_pipe, O_RDWR);
    if (fd == -1){
        printf("\n ocurreu um erro ao abrr o pipe \n");
        return FALSE;
    }
    return fd;
}

int verifica_conecao(){
    if (access(PIPE_DIR, F_OK) == -1){
        printf("\n O Servidor nao se encontra em execucao! \n");
        exit(0);
    }

    int fd = open(PIPE_DIR, O_RDWR);
    if (fd == -1){
        printf("\n ocurreu um erro ao abrr o pipe do servidor \n");
        exit(0);
    }
    return fd;
}

void update_info(JOGADORES recebe){
    if(recebe.action == RECEBE_PONTOS){
        strcpy(jogo.jogando, recebe.jogando);
        if (recebe.pontos != 0){
            jogo.pontos = jogo.pontos + recebe.pontos;
        }
    }
}

void fechar_cliente(){
    int fd_envia = verifica_conecao();
    if (fd_envia == FALSE){
        exit(0);//fazer
    }
    jogo.action = SAIR;
    write(fd_envia, &jogo, sizeof(jogo));
}

void envia_ao_server(){
    int fd_envia = verifica_conecao();
    write(fd_envia, &jogo, sizeof(jogo));
    //close(fd_envia);
}

void* recebe_info(){        //envia sempre o &jogo mas se receber trabalha se com recebe
    JOGADORES recebe, envia;

    int fd_recebe, fd_envia;    //  recebe pipe_cliente    /       envia pipe_server
    char pipe_cliente[30];

    fd_envia = verifica_conecao();
    fd_recebe = cria_pipe_cliente(pipe_cliente);

    jogo.pid_jogador = getpid();
    jogo.action = LOG_IN;

    write(fd_envia, &jogo, sizeof(jogo));

    while (TRUE){
        memset(&recebe, 0, sizeof(JOGADORES) );
        memset(&envia, 0, sizeof(JOGADORES) );

        read(fd_recebe, &recebe, sizeof(recebe));
        update_info(recebe);
        
        if (recebe.action == ENTROU){
            printf("Bem vindo: %s\n\n", jogo.nome);

        }else if (recebe.action == SAIU){   //saida e fecho de cliente
            unlink(pipe_cliente);
            close(fd_recebe);
            close(fd_envia);
            exit(0);

        }else if (recebe.action == SAIR_SERVIDOR){
            printf("\nO servidor foi desligado\n\nO programa vai fechar!\n");
            close(fd_envia);
            close(fd_recebe);
            unlink(pipe_cliente);
            exit(0);
        }else if(recebe.action == NOME_JA_EXISTE){
            printf("\nJá existe um cliente a utilizar o seu nome!");
            printf("O programa vai encerrar!!\n");
            unlink(pipe_cliente);
            exit(0);
        }else if(recebe.action == DESISTE){
            printf("\nDesistiu do Campeonato.\nO Programa irá encerrar!\n");

            envia.action = DESISTIU;
            write(fd_envia, &envia, sizeof(envia));

            close(fd_envia);
            close(fd_recebe);
            unlink(pipe_cliente);
            exit(0);
        }else if(recebe.action = PERGUNTA_DO_JOGO){
            jogando = 1;
            printf("%s", recebe.pergunta_do_jogo);
        }
    }
}