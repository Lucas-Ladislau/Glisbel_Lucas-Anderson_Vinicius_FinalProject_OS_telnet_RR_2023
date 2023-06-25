#include "error_handling.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


int change_port_client(char *new_port) {
  int str_size = strlen(new_port) - 7;

  char port[str_size + 1];

  strncpy(port, &new_port[7], str_size);
  port[str_size] = '\0';

  return strtol(port, NULL, 10);
}