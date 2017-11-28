//=============================================================================
// SERVIDOR.C
// Exemplo de uso do mecanismo de comunicação por Socket - UDP
// Programa que recebe e envia uma mensagem para outro processo via socket
// Rafaela Gomonth de Miranda
// Victo Ferreira Lima
//=============================================================================

#include "./local.h"
#include "./help.h"
#include <sys/utsname.h>
#include <time.h>
#include <string.h>
main(int argc, char *argv[])
{
  struct utsname uts;
  uname(&uts);

  int socket_servidor, socket_cliente, bytes_recebidos, bytes_enviados;
  struct sockaddr_in servidor, cliente;
  int tamanho_cliente = sizeof(cliente);
  int tamanho_servidor = sizeof(servidor);
  char buffer[MAX_SIZE_BUFFER];

  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  int month = timeinfo->tm_mon;
  month = month + 1;
  int year = timeinfo->tm_year;

  printf("------------------------------------------------------\n");
  printf("Servidor aguardando comandos pela porta %d\n", atoi(argv[1]));
  printf("------------------------------------------------------\n");

  socket_servidor = socket(AF_INET, SOCK_DGRAM, 0);

  bzero(&servidor, sizeof(servidor));
  servidor.sin_family = AF_INET;
  servidor.sin_port = htons(atoi(argv[1]));
  servidor.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(socket_servidor, (struct sockaddr *)&servidor, tamanho_servidor);

  while (strcmp(buffer, "shutdown") != 0)
  {
    bytes_recebidos = recvfrom(socket_servidor, buffer, MAX_SIZE_BUFFER, 0, (struct sockaddr *)&cliente, &tamanho_cliente);
    printf("Comando a ser processado: %s.\n", buffer);

    if (strcmp(buffer, "date") == 0)
    {
      snprintf(buffer, sizeof(buffer), "%d/%d/%d", timeinfo->tm_mday, month, year + 1900);
    }

    else if (strcmp(buffer, "time") == 0)
    {
      snprintf(buffer, sizeof(buffer), "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    }
    else if (strcmp(buffer, "nodename") == 0)
    {
      strcpy(buffer, ("Node name:   %s", uts.nodename));
    }
    else if (strcmp(buffer, "sysname") == 0)
    {
      strcpy(buffer, ("System name: %s", uts.sysname));
    }
    else if (strcmp(buffer, "release") == 0)
    {
      strcpy(buffer, ("Release:    %s", uts.release));
    }
    else if (strcmp(buffer, "version") == 0)
    {
      strcpy(buffer, ("Version:     %s", uts.version));
    }

    else if (strcmp(buffer, "machine") == 0)
    {
      strcpy(buffer, ("Machine:     %s", uts.machine));
    }
    
    else if (strcmp(buffer, "credits") == 0)
    {
      strcpy(buffer, "### Versão de terminal linux desenvolvido para a matéria de Redes\n### Copyright by Rafaela Gomonth de Miranda e Victo Ferreira Lima");
    }

    else if((strcmp(buffer, "date") != 0) && (strcmp(buffer, "time") != 0) && (strcmp(buffer, "nodename") != 0) && (strcmp(buffer, "sysname") != 0) && (strcmp(buffer, "release") != 0) && (strcmp(buffer, "version") != 0) && (strcmp(buffer, "machine") != 0) && (strcmp(buffer, "shutdown") != 0) && (strcmp(buffer, "credits") != 0) && (strcmp(buffer, "exit") != 0) && (strcmp(buffer, "help") != 0))
    {
      strcpy(buffer, "Comando não encontrado ou invalido");
    }

    bytes_enviados = sendto(socket_servidor, buffer, MAX_SIZE_BUFFER, 0, (struct sockaddr *)&cliente, tamanho_cliente);
  }
  close(socket_servidor);
  return 0;
}