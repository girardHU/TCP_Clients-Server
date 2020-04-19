#include "connect.h"

int main(int argc, char** argv) {
  int                 serv_sockfd, new_sockfd, clients_sock[MAXCLIENTS], max_fd, fd, i;
  struct sockaddr_in  serv_addr, client_addr;
  socklen_t           client_addr_len;

  // Initialize client sockets to 0
  for (i = 0; i < MAXCLIENTS; i++) {
    clients_sock[i] = 0;
  }

  fd_set readfs;
  // struct timeval timeout;

  if (argc != 2)
    err_n_die(NULL, "usage: %s <server_port>\n", argv[0]);

  if ( (serv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die(NULL, "Error upon socket creation\n");
  else
    printf("socket created\n");

  bzero(&serv_addr, sizeof(serv_addr));
  // assign address and port
  serv_addr.sin_family       = AF_INET;
  serv_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
  serv_addr.sin_port         = htons(atoi(argv[1]));

  if ( (bind(serv_sockfd, (SA*) &serv_addr, sizeof(serv_addr))) != 0)
    err_n_die(&serv_sockfd, "Error upon binding\n");
  else
    printf("socket binded\n");

  if ( (listen(serv_sockfd, MAXCLIENTS)) != 0)
    err_n_die(&serv_sockfd, "Error upon listening\n");
  else
    printf("server listening on port %s\n", argv[1]);
  client_addr_len = sizeof(client_addr);
  
  while (TRUE) {
    FD_ZERO(&readfs);

    FD_SET(serv_sockfd, &readfs);
    max_fd = serv_sockfd;
    for (int i = 0; i < MAXCLIENTS; i++) {
      fd = clients_sock[i];
      if (fd > 0)
        FD_SET(fd, &readfs);
      if(fd > max_fd)
        max_fd = fd;
    }

    if ( select(max_fd + 1, &readfs, NULL, NULL, NULL) < 0)
      printf("select error\n");

    if (FD_ISSET(serv_sockfd, &readfs)) {
      if ( (new_sockfd = accept(serv_sockfd, (SA*) &client_addr, &client_addr_len)) < 0)
        err_n_die(&serv_sockfd, "Error upon new accept\n");
      
      printf("new connection attempt!\n");

      // store new connection in an empty slot
      server_storeClient(clients_sock, &new_sockfd);
    }

    for (i = 0; i < MAXCLIENTS; i++) {
      fd = clients_sock[i];
      if (FD_ISSET(fd, &readfs))
        server_clientRead(clients_sock, i);
    }
    printf("one loop\n\n");
  }
  close(serv_sockfd);
  return (0);
}