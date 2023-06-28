#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../client_utils/socket_options.h"
#include "../client_utils/host_options.h"

void server_possible_flags(int argc, char *argv[], int *PORT,char **HOST, int *COMPRESS) {
  for (int i = 1; i < argc; i++) {
    if (strstr(argv[i], "--port") != NULL){
      *PORT = change_port(argv[i]);
    }else if(strstr(argv[i], "--compress") != NULL){
       *COMPRESS = 1;
    }else if(strstr(argv[i], "--host") != NULL){
       *HOST = change_host(argv[i]);
    }

  }
}