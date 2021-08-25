#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>

struct sockaddr_in address, client_address;
int sock_server, sock_tmp, address_len;
char buffer[101];

void config_address(char **other_end_address){
    address.sin_family = client_address.sin_family  = AF_INET;
    address.sin_addr.s_addr                  = inet_addr(other_end_address[1]);
    address.sin_port                         = htons(atoi(other_end_address[2]));
}

void config_socket(){
    if ((sock_server = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket:");
        exit(1);
    }
    if (bind(sock_server, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind:");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Coloque como argumento apenas o ip e a porta serem usados!!\n");
        return 1;
    }

    config_address(argv);
    config_socket();

    if (listen(sock_server, 3) < 0){
        perror("listen:");
        exit(1);
    }

    address_len = sizeof(address);
    if ((sock_tmp = accept(sock_server, (struct sockaddr *) &address, &address_len)) < 0){
        perror("Accept:");
        exit(1);
    }

    int tmp = read(sock_tmp, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    strcpy(buffer, "100\0");
    send(sock_tmp, buffer, strlen(buffer), 0);
    return 0;
}