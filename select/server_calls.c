#include "connect.h"

/**
 * DESC:           takes an array of sockets and a single socket and check if it can store this socket in an empty slot of the array
 * 
 * PARAMETERS:
 *      int* sock_array           array of sockets
 *      int* new_sock             new socket that needs to be stored
 * 
 * RETURN:
 *  Type int
 *  Values:
 *         -3 -> fd sent was not opened
 *         -1 -> no slot available in sock_array
 *          0 -> success
 */
int server_storeClient(int* sock_array, int* new_sock) {
    int i;

    if (*new_sock <= 0)
      return (-3);

    for (i = 0; i < MAXCLIENTS; i++) {
    if (sock_array[i] == 0) {
      sock_array[i] = *new_sock;
      printf("New socket is referenced as the %d\n", i);
      return (0);
    }
  }
  printf("No slot available\n");
  // have to check return of send
  server_alertNoSlot(new_sock);
  return (-1);
}

/**
 * DESC:           takes an array of sockets and a reference and close the socket at this reference in this array
 * 
 * PARAMETERS:
 *      int* sock_array           array of sockets
 *      int ref                   reference of the socket to close in the array
 * 
 * RETURN:
 *  Type int
 *  Values:
 *          1 -> fd selected was not opened
 *          0 -> success
 */
int server_closeClient(int* sock_array, int ref) {
  if (sock_array[ref] == 0)
    return (1);

  printf("client on reference %d just disconnected\n", ref);
  close(sock_array[ref]);
  sock_array[ref] = 0;
  return (0);
}

/**
 * DESC:           takes a pointer on a socket (fd), try to reacv a message from it, displayes it and send back call the server_respClient with a "OK\n" string in param
 * 
 * PARAMETERS:
 *      int* client               pointer on an int being a file descriptor of a socket
 *      int ref                   reference of the client in the socket array
 * 
 * RETURN:
 *  Type int
 *  Values: 
 *          1 -> fd given was not opened
 *         -1 -> could not send the message
 *         -2 -> client has disconnected
 *          0 -> success
 */
int server_readClient(int* sock_array, int ref) {
  // printf("start server_readClient\n");
  int recvline, n, i;

  recvline = 0;
  n = 0;

  if (sock_array[ref] == 0)
    return (1);

  if ( (n = recv(sock_array[ref], &recvline, MAXBITS, 0) == 0)) {
    server_closeClient(sock_array, ref);
    return (-2);
  }
  if (n == -1) {
    printf("error upon receiving from the client\n");
    return (-1);
  }
    
  printf("received %d\n", recvline);

  for (i = 0; i < MAXCLIENTS; i++)
    server_respClient(sock_array, i, &recvline);

  // printf("end server_readClient\n");
  return (0);
}

/**
 * DESC:           takes a pointer on a socket (fd) and a message and send it to that socket
 * 
 * PARAMETERS:
 *      int* client               pointer on an int being a file descriptor of a socket
 *      char* sendline            string to be sent to the passed socket
 * 
 * RETURN:
 *  Type int
 *  Values: 
 *          1 -> fd given was not opened
 *         -1 -> could not send the message
 *          0 -> success
 */
int server_respClient(int* sock_array, int ref, int* sendline) {
  if (sock_array[ref] == 0)
    return (1);

  if ( send(sock_array[ref], sendline, sizeof(*sendline), MSG_NOSIGNAL) == -1) {
    server_closeClient(sock_array, ref);
    perror("can't respond to the client ");
    return (-1);
  }

  return (0);
}

/**
 * DESC:           takes a pointer on a socket (sock), tries to write a message on it then close it, and if cannot close it anyway
 * 
 * PARAMETERS:
 *      int* sock                 pointer on an int being a file descriptor of a socket
 * 
 * RETURN:
 *  Type int
 *  Values: 
 *          1 -> fd given was not opened
 *         -1 -> could not send the message
 *          0 -> success
 */
int server_alertNoSlot(int* sock) {
  char* msg = "No slot available, sorry\n";

  if (*sock == 0)
    return (1);

  if ( send(*sock, msg, sizeof(msg), 0) == -1) {
    close(*sock);
    return (-1);
  }
  
  printf("alerted the client successfully\n");
  close(*sock);
  return (0);
}