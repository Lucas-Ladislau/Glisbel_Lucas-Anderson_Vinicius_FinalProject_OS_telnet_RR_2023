#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* change_host(char* arg) {
  
  char* equals_sign = strchr(arg, '=');
  char* host = equals_sign + 1;

  return strdup(host);
}
