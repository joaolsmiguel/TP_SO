/*
TRABALHO CRIADO E DESENVOLVIDO POR:
JOAO MIGUEL - 2018018852
MARIO GARRAO - 2017013216
*/

#include "comandos.h"

void get_nome(char* cmd, char* nome){
    char aux_nome[strlen(cmd)-1];
    int i = 1;

    while (cmd[i] != '\0'){
        aux_nome[i-1] = cmd[i];
        i++;
    }
    aux_nome[i-1] = '\0';
    
    strcpy(nome, aux_nome);
}

int bloqueia_comms_jogador(char* nome){
    if (verifica_jogador_bloqueado(nome) >= 0){
        printf("\njogador já bloqueado\n");
        return 0;
    }else{
        strcpy(bloq_comms[n_bloq_comms], nome);
        n_bloq_comms++;
        printf("\njogador bloqueado\n");
        return 0;
    }
}

int verifica_jogador_bloqueado(char* nome){
    for (int i = 0; i <= n_bloq_comms; i++){
        if (strcmp(nome, bloq_comms[i]) == 0){
            return i;
        }
    }
    return -1;
}

void printf_bloq(){
    for (int i = 0; i < n_bloq_comms; i++){
        printf("\nbloq[%i] %s",i, bloq_comms[i]);
    }
}

int desbloqueia_comms_jogador(char* nome){
    int n_pos_bloq = verifica_jogador_bloqueado(nome);

    if (n_pos_bloq >= 0){
        for (int i = 0; i <= n_bloq_comms; i++){
            if (i == n_pos_bloq && n_pos_bloq < n_bloq_comms){
                for (int j = i; j < n_bloq_comms; j++){
                    strcpy(bloq_comms[j], bloq_comms[j + 1]);
                }
            }else if(i == n_pos_bloq && n_pos_bloq == n_bloq_comms){
                bloq_comms[i][0] = '\0';
            }
        }
        n_bloq_comms--;
        return 1;
    }else{
        return 0;
    }
}


void verifica_cmd(char* cmd){
    if (cmd[0] == 'k' || cmd[0] == 'K'){        //done
        char* nome;
        get_nome(cmd, &nome);
        int existe_jogador = verifica_nome_existe(&nome);
        if (existe_jogador >= 0){
            JOGADORES envia;
            envia.action = DESISTE;
            envia_para_cliente(existe_jogador, envia);
            apaga_user(players[existe_jogador].pid_jogador);
        }
    }else if (cmd[0] == 'S' || cmd[0] == 's'){      //done
        char* nome;
        get_nome(cmd, &nome);
        if(verifica_nome_existe(&nome) != -1){
            bloqueia_comms_jogador(&nome);
        }
    }else if (cmd[0] == 'R' || cmd[0] == 'r'){      //done
        char* nome;
        get_nome(cmd, &nome);
        if (verifica_nome_existe(&nome) != -1){
            desbloqueia_comms_jogador(&nome);
        }
    }else if (strcmp(cmd, "players") == 0){       //done
        print_active_players();      
    }else if (strcmp(cmd, "games") == 0){       //done
        verifica_jogos_existentes();
    }else if (strcmp(cmd, "end") == 0){     //done
        if (em_campeonato == 1){
            acaba_campeonato();
        }
    }else if (strcmp(cmd, "exit") == 0){        //done
        fecha_server();
    }else if(strcmp(cmd, "pbloq") == 0){
        printf_bloq();
    }
}
