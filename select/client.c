#include "connect.h"

int main(int argc, char** argv) {
  int                 sockfd;
  struct sockaddr_in  serv_addr;
  char                sendline[MAXBITS];
  char                recvline[MAXBITS];

  fd_set readfs;
  fd_set writefs;

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

  while (TRUE) {
    FD_ZERO(&readfs);
    FD_ZERO(&writefs);
    memset(recvline, '\0', MAXBITS);
    memset(sendline, '\0', MAXBITS);

    // FD_SET(sockfd, &readfs);
    // FD_SET(sockfd, &writefs);

    // if ( select(sockfd + 1, &readfs, NULL, NULL, NULL) < 0)
    //   printf("select error\n");

    // We're connected. read message from stdin
    printf("\nType your message then press enter :\n");
    read(0, sendline, MAXBITS);

    // Send the request -- making sure you send it all
    if (sendline[0] != '\0')
      if ( client_requestServer(&sockfd, sendline) == -1)
        break;

    // Now read the server's response
    if ( client_readServer(&sockfd) == -1)
      break;
  }

  printf("closing...\n");
  close(sockfd);
  exit(0); // end successfully
}