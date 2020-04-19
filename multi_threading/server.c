#include "connect.h"

int main(int argc, char** argv) {
  int                 serv_sockfd;
  struct sockaddr_in  serv_addr; //, client_addr;
  // socklen_t           client_addr_len;
  connection_t*       connection;
  pthread_t           thread;

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
  // client_addr_len = sizeof(client_addr);
  
  while (TRUE) {

    connection = (connection_t *)malloc(sizeof(connection_t));

    connection->sock = accept(serv_sockfd, &connection->address, &connection->addr_len);
    if (connection->sock <= 0) {
      free(connection);
    } else {
      /* start a new thread but do not wait for it */
      pthread_create(&thread, NULL, connection_handler, (void *)connection);
      pthread_detach(thread);
    }
  }
  close(serv_sockfd);
  return (0);
}