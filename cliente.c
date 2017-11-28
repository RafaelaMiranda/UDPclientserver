//=============================================================================
// CLIENTE.C
// Exemplo de uso do mecanismo de comunicação por Socket - UDP
// Programa que envia e recebe uma mensagem para outro processo via socket
// Rafaela Gomes de Miranda
// Victo Ferreira Lima
//=============================================================================
#include "./local.h"
#include "./help.h"

#include <sys/utsname.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

main(int argc, char *argv[])
{
  struct utsname uts;
  uname(&uts);

  int socket_servidor, bytes_enviados, bytes_recebidos;
  struct sockaddr_in servidor;
  int tamanho_servidor = sizeof(servidor);
  char buffer[MAX_SIZE_BUFFER];

  while (strcmp(buffer, "shutdown") != 0)
  {

    // DESCOBRINDO IP
    unsigned char ip_address[15];
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    memcpy(ifr.ifr_name, "wlp5s0", IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    strcpy(ip_address, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    socket_servidor = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(atoi(argv[2]));
    servidor.sin_addr.s_addr = inet_addr(argv[1]);

    printf("%s:%d @: ", ip_address, atoi(argv[2]));
    gets(buffer);

    bytes_enviados = sendto(socket_servidor, buffer, MAX_SIZE_BUFFER, 0, (struct sockaddr *)&servidor, sizeof(servidor));

    bytes_recebidos = recvfrom(socket_servidor, buffer, MAX_SIZE_BUFFER, 0, (struct sockaddr *)&servidor, &tamanho_servidor);

    if (strcmp(buffer, "help") == 0)
    {
      help();
    }

    if (strcmp(buffer, "exit") == 0)
    {
      exit(EXIT_SUCCESS);
    }

    if (strcmp(buffer, "shutdown") != 0 && strcmp(buffer, "help") != 0)
    {
      printf("%s\n\n", buffer);
    }

    close(socket_servidor);
  }
  return 0;
}