#ifndef _CONNECT_H_
#define _CONNECT_H_

#include <sys/socket.h> /* basic socket definition */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> /* for inet_pton, and the like */
#include <stdarg.h> /* for variable argument functions, like err_n_die */
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <fcntl.h>
// #include <netdb.h>
#include <pthread.h>

#define MAXBITS 128
#define SA struct sockaddr
#define MAXCLIENTS 3
#define TRUE 1

typedef struct
{
    int sock;
    struct sockaddr address;
    socklen_t addr_len;
} connection_t;

void err_n_die(int* sock, const char* fmt, ...);

// server_calls.c
int server_storeClient(int* sock_array, int* new_sock);
int server_clientClose(int* sock_array, int ref);
int server_clientResp(int* sock_array, int ref, char* sendline);
int server_clientRead(int* sock_array, int ref);
int server_alertNoSlot(int* sock);

void* connection_handler(void* ptr);

// client_calls.c
int client_requestServer(int* server_socket, char* request_body);
int client_readServer(int* server_socket);

#endif