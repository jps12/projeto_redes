/***********************************/
/* FGA/EngSofware/FRC  udpClient.c */
/* Prof. Fernando W Cruz           */
/***********************************/ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>

#define MAX_MSG 100
int sd, rc, i;
struct sockaddr_in ladoCli;   /* dados do cliente local   */
struct sockaddr_in ladoServ; 	/* dados do servidor remoto */

int get_message_size(){
  char msg[50] = "?";
  rc = sendto(sd, msg, strlen(msg), 0,(struct sockaddr *) &ladoServ, sizeof(ladoServ));
  if (rc < 0){
    printf("Nao ta aceitando contato\n");
  }
  printf("Deu bom\n");
  int tmp, client_server = socket(AF_INET, SOCK_DGRAM, 0);
  rc = bind(client_server, (struct sockaddr *) &ladoCli,sizeof(ladoCli));
  if (rc < 0){
    printf("Não foi possível fazer o bind\n");
    exit(1);
  }
   printf("IP addres: %s and porta usada: %u\n", inet_ntoa(ladoCli.sin_addr), ntohs(ladoCli.sin_port));
  int tam_ladoServ = sizeof(ladoServ);
  tmp = recvfrom(client_server, msg, strlen(msg), 0, (struct sockaddr *) &ladoServ, &tam_ladoServ);

  if (tmp < 0){
    printf("Deu ruim com a espera\n");
    exit(1);
  }
  close(client_server);
  printf("Tamanho da mensagem: %d\n", atoi(msg));
}


int main(int argc, char *argv[]) {
  /* confere o numero de argumentos passados para o programa */
  if(argc<3)  {
    printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(1);  }

/* Preenchendo as informacoes de identificacao do remoto */
  ladoServ.sin_family 	   = AF_INET;
  ladoServ.sin_addr.s_addr = inet_addr(argv[1]);
  ladoServ.sin_port 	   = htons(atoi(argv[2]));

/* Preenchendo as informacoes de identificacao do cliente */
  ladoCli.sin_family 	 = AF_INET;
  ladoCli.sin_addr.s_addr= htonl(INADDR_ANY);  // descobrir o localhost
  ladoCli.sin_port 	     = htons(0); /* usa porta livre entre (1024-5000)*/

/* Criando um socket. Nesse momento a variavel       */
/* sd contem apenas dados sobre familia e protocolo  */

  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: n�o pode abrir o socket \n",argv[0]);
    exit(1); }

/* Relacionando o socket sd com a estrutura ladoCli /*
/* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
  rc = bind(sd, (struct sockaddr *) &ladoCli, sizeof(ladoCli));
  if(rc<0) {
    printf("%s: n�o pode fazer um bind da porta\n", argv[0]);
    exit(1); }
  printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: %s, Porta_R: %s}\n", inet_ntoa(ladoCli.sin_addr), ntohs(ladoCli.sin_port), argv[1], argv[2]);

  get_message_size();
  return 0;
  /* Enviando um pacote para cada parametro informado */
  for(i=3;i<argc;i++) {
    rc = sendto(sd, argv[i], strlen(argv[i]), 0,(struct sockaddr *) &ladoServ, sizeof(ladoServ));
    if(rc<0) {
      printf("%s: nao pode enviar dados %d \n",argv[0],i-1);
      close(sd);
      exit(1); }
    printf("Enviando parametro %d: %s\n", i-2, argv[i]);
  } /* fim do for (laco) */
  return 1;
} /* fim do programa */
