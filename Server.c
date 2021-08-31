#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 54321

struct sockaddr_in address, client_address;
int sock_server, sock_tmp, address_len, msgid;
char buffer[101];
key_t key;

struct messeg_buffer
{
    long message_type;
    char message_text[500];
}message;

void config_queue();
void config_address(char **other_end_address);
void config_socket();
void send_to_queue();
void recieve_message();

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Coloque como argumento apenas o ip e a porta serem usados!!\n");
        return 1;
    }

    config_address(argv);
    config_socket();
    config_queue();

    
    while (1){
        recieve_message();
        send_to_queue();
    }
}

void config_queue(){
    key = ftok("Server.c", KEY);
    if (key < 0){
        perror("Criando chave:");
        exit(1);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.message_type = 1;
}

void config_address(char **other_end_address){
    address.sin_family = client_address.sin_family  = AF_INET;
    address.sin_addr.s_addr                         = inet_addr(other_end_address[1]);
    address.sin_port                                = htons(atoi(other_end_address[2]));
}

void config_socket(){
    if ((sock_server = socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("socket:");
        exit(1);
    }
    if (bind(sock_server, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind:");
        exit(1);
    }
}

void send_to_queue(){
    if (msgsnd(msgid, &message, sizeof(message), 0) < 0){
        perror("Mandando mensagem: ");
    }
}

void recieve_message(){
    int client_address_len = sizeof(client_address), tmp;
    tmp = recvfrom(sock_server, message.message_text, sizeof(message.message_text), 0, (struct sockaddr *) &client_address, &client_address_len);

    if (tmp < 0){
        perror("Receber mensagem: ");
        exit(1);
    }
}
