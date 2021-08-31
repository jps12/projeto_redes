#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 54321

key_t key;
int msgid;

struct messeg_buffer
{
    long message_type;
    char message_text[500];
}message;

void config_queue();
void recieve_from_queue();

int main(){
    config_queue();
    while (1){
        message.message_type = 1;
        printf("Esperando mensagem do servido...\n");
        recieve_from_queue();
        printf("Mensagem recebida: %s\n", message.message_text);
    }
}

void config_queue(){

    key = ftok("Server.c", KEY);
    if (key < 0){
        perror("Criando chave");
        exit(1);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
}

void recieve_from_queue(){
    if (msgrcv(msgid, &message, sizeof(message), 1, 0) < 0){
        perror("Receber mensagem da fila:");
        exit(1);
    }
}