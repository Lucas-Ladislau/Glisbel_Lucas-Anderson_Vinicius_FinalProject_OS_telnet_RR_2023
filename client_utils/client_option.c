#include "log_option.h"
#include "socket_options.h"
#include "host_options.h"
#include <stdio.h>
#include <string.h>

void client_possible_flags(int argc, char *argv[], char **LOG_NAME, int *PORT, int *LOG_SAVE, char **HOST) {
  for (int i = 1; i < argc; i++) {

    if (strstr(argv[i], "--log") != NULL) {
      *LOG_SAVE = 1;
      *LOG_NAME = create_log(argv[i], *PORT);
    } 
    
    else if (strstr(argv[i], "--port") != NULL)
      *PORT = change_port_client(argv[i]);

    else if (strstr(argv[i], "--host") != NULL)
      *HOST = change_host(argv[i]);

  }
}