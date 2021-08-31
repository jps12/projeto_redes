#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 12134

key_t key;
int msgid;

struct messeg_buffer
{
    long message_type;
    char message_text[500];
}message;

void config_queue();
void send_to_queue();

int main(){
    
    config_queue();

    while (1){
        printf("Digite a mensagem a ser enviada: ");
        scanf(" %s", message.message_text);
        printf("Enviando messagem...\n");
        send_to_queue();
    }
}

void config_queue(){
    key = ftok("Client.c", KEY);
    if (key < 0){
        perror("Criando chave");
        exit(1);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.message_type = 1;
}

void send_to_queue(){
    if (msgsnd(msgid, &message, sizeof(message), 0) < 0){
        perror("Mandando mensagem: ");
    }
    printf("Mensagem enviada!!!\n");
}