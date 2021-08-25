#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>

struct sockaddr_in address, server_address;
int sock;

void config_address(char **other_end_address){
    address.sin_family = server_address.sin_family  = AF_INET;
    server_address.sin_addr.s_addr                  = inet_addr(other_end_address[1]);
    server_address.sin_port                         = htons(atoi(other_end_address[2]));

    address.sin_addr.s_addr  = htonl(INADDR_ANY);  // descobrir o localhost
    address.sin_port 	     = htons(0); /* usa porta livre entre (1024-5000)*/
}

void config_socket(){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket:");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind:");
        exit(1);
    }
}

int get_tam_message(){
    char msg[10] = "?";
    int server_len = sizeof(server_address), tmp;
    tmp = connect(sock, (struct sockaddr *) &server_address, server_len);
    if (tmp < 0){
        perror("get message size connect:");
        exit(1);
    }
    send(sock, msg, strlen(msg), 0);
    printf("Asked message size\n");
    tmp = read(sock, msg, 10);
    printf("%s\n", msg);
    return atoi(msg);
}

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Aceitamos apenas o ip do server e a port que ele usa!! (2 argumentos)\n");
        exit(0);
    }
    config_address(argv);
    config_socket();

    while (1){
        char msg[200];
        int tam_msg = get_tam_message();
        // printf("Digite a mensagem a ser mandada(max 200 caracteres): ");
        // scanf(" %s", msg);
        // send_message(msg, tam_msg);
    }
}