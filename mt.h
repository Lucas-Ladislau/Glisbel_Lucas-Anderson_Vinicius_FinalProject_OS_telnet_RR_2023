#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#define MAX_MSG_LENGTH 1024
#define END_STRING "encerrou\n"
#define COMPLETE_STRING "fim-resposta"

#ifndef MSG_NOSIGNAL
#endif

#define perro(x) {fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, x, strerror(errno));exit(1);}
