CC=			gcc

SRC=		server_calls.c \
				client_calls.c \
				err_n_die.c

#OBJ=		$(SRC:%.c=.o)

CFLAGS=	-Wall -Wextra -Werror

all:	server client

client:
	$(CC) $(CFLAGS) $(SRC) client.c -o client

server:
	$(CC) $(CFLAGS) $(SRC) server.c -o server

clean:
	rm -f *.o

fclean: clean
	rm -f client server
	rm -f a.out

re: fclean all