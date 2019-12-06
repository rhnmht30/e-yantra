#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

int main()
{
  int sock, cli;
  struct sockaddr_in server, client;
  int len;
  char msg[] = "Welcome Fuckers!";
  char rcv[1024] = {0};
  int sent, rec;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket: ");
    exit(-1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(3333);
  server.sin_addr.s_addr = INADDR_ANY;
  bzero(&server.sin_zero, 0);

  len = sizeof(struct sockaddr_in);

  if ((bind(sock, (struct sockaddr *)&server, len)) == -1)
  {
    perror("bind: ");
    exit(-1);
  }

  if ((listen(sock, 5)) == -1)
  {
    perror("listen: ");
    exit(-1);
  }

  while (1)
  {
    if ((cli = accept(sock, (struct sockaddr *)&client, &len)) == -1)
    {
      perror("accept: ");
      exit(-1);
    }

    sent = send(cli, msg, strlen(msg), 0);
    printf("sent %d bytes to client: %s\n", sent, inet_ntoa(client.sin_addr));
    //wait

    rec = recv(cli, rcv, sizeof(rcv) - 1, 0);
    if (rec)
    {

      printf("recvd %d bytes from client: %s is: %s\n", rec, inet_ntoa(client.sin_addr), rcv);
      memset(rcv, 0, 1024);
      close(cli);
    }

    // shutdown(cli, 2);
  }
  return 0;
}