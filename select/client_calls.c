#include "connect.h"

/**
 * DESC:           takes a `server_socket` and tries to write `request_body` on it
 * 
 * PARAMETERS:
 *      int* server_socket        socket of the server
 *      char* request_body        body of the message to write on the socket
 * 
 * RETURN:
 *  Type int
 *  Values:
 *         -1 -> server is down (EPIPE)
 *          1 -> socket was not opened
 *          0 -> success
 */
int client_requestServer(int* server_socket, char* request_body) {
  if (*server_socket <= 0)
    return (1);

  if (send(*server_socket, request_body, sizeof(request_body), MSG_NOSIGNAL) < 0)
    err_n_die(server_socket, "write error!\n");
  else if (errno == EPIPE) {
    printf("server down...\n");
    return (-1);
  }
  printf("message sent\n");
  return (0);
}

/**
 * DESC:           takes a `server_socket` and tries to read from it
 * 
 * PARAMETERS:
 *      int* server_socket        socket of the server
 * 
 * RETURN:
 *  Type int
 *  Values:
 *         -1 -> server has shutdown the connection
 *          1 -> error while receiving, check errno for more infos
 *          0 -> success
 */
int client_readServer(int* server_socket) {
  int n = -1;
  char msg_received[MAXBITS];

  if ( (n = recv(*server_socket, msg_received, MAXBITS, 0)) == 0) {
    printf("connection lost!\n");
    return (-1);
  } else if (n < 0) {
    perror("No message received from the server ");
    return (1);
  }
  printf("received from server : %s\n", msg_received);
  return (0);
}