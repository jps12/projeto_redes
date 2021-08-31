#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 12134

struct sockaddr_in address, server_address;
int sock, msgid;
key_t key;

struct messeg_buffer
{
    long message_type;
    char message_text[500];
}message;

void config_queue();
void config_address(char **other_end_address);
void config_socket();
void send_message();
void wait_from_queue();

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Aceitamos apenas o ip do server e a port que ele usa!! (2 argumentos)\n");
        exit(0);
    }
    
    config_address(argv);
    config_socket();
    config_queue();

    while (1){
        wait_from_queue();
        send_message();
    }
    return 0;
}

void config_queue(){
    key = ftok("Client.c", KEY);
    msgid = msgget(key, 0666 | IPC_CREAT);
    
}

void config_address(char **other_end_address){
    address.sin_family = server_address.sin_family  = AF_INET;
    server_address.sin_addr.s_addr                  = inet_addr(other_end_address[1]);
    server_address.sin_port                         = htons(atoi(other_end_address[2]));

    address.sin_addr.s_addr  = htonl(INADDR_ANY);  // descobrir o localhost
    address.sin_port 	     = htons(0); /* usa porta livre entre (1024-5000)*/
}

void config_socket(){
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("socket:");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind:");
        exit(1);
    }
}

void send_message(){
    int tmp = sendto(sock, message.message_text, strlen(message.message_text), 0, (const struct sockaddr *) &server_address, sizeof(server_address));
    if (tmp < 0){
        perror("Send to:");
    }
}

void wait_from_queue(){
    message.message_type = 1;
    if (msgrcv(msgid, &message, sizeof(message), 1, 0) < 0){
        perror("Receber mensagem da fila:");
        exit(1);
    }
}