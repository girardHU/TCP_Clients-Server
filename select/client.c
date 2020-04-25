#include "connect.h"

int main(int argc, char** argv) {
  int                 sockfd, rotation;
  struct sockaddr_in  serv_addr;
  char                sendline[MAXBITS];
  char                recvline[MAXBITS];

  fd_set readfs;

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  if (argc != 3)
    err_n_die(NULL, "usage: %s <server_address> <server_port>\n", argv[0]);

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die(NULL, "Error while creating the socket!\n");
  else
    printf("socket created\n");

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));

  // translate address
  if ( inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    err_n_die(&sockfd, "inet_pton error for %s\n", argv[1]);
  else
    printf("address translated\n");

  // connect to the server
  if (connect(sockfd, (SA *) &serv_addr, sizeof(serv_addr)) < 0)
    err_n_die(&sockfd, "connect failed!\n");
  else
    printf("connected to the server\n");

  // // put the socket in non blocking mode
  // if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0)
  //   err_n_die(&sockfd, "could not set the socket on a non blocking mode\n");

    rotation = 0;

  while (TRUE) {
    FD_ZERO(&readfs);
    memset(recvline, '\0', MAXBITS);
    memset(sendline, '\0', MAXBITS);

    FD_SET(sockfd, &readfs);

    if ( select(sockfd + 1, &readfs, NULL, NULL, &timeout) < 0)
      printf("select error\n");

    // Send the request -- making sure you send it all
    if ( client_requestServer(&sockfd, &rotation) == -1)
      break;

    // Now read the server's response
    if (FD_ISSET(sockfd, &readfs)) {
      if ( client_readServer(&sockfd) == -1)
        break;
    }
    rotation++;
    sleep(1);
  }

  printf("closing...\n");
  close(sockfd);
  exit(0); // end successfully
}