#include "mt.h"
#include "client_utils/client_option.h"
#include "client_utils/log_option.h"
#include "client_utils/error_handling.h"
#include "compress_utils/compress_functions.h"

char *HOST = "127.0.0.1";

int COMPRESS =0; 
int PORT = 4444; // default port
int LOG_SAVE = 0;  
char *LOG_NAME = NULL;
int SEND_OR_RECEIVE = 0;
unsigned long command_size;
unsigned long command_byte_size;   


void send_cmd(int sock, int pid) {
    char str[MAX_MSG_LENGTH];
    printf("\033[32m> ");
    while (fgets(str, MAX_MSG_LENGTH, stdin) == str) {
        if (strncmp(str, END_STRING, strlen(END_STRING)) == 0) break;
        if (send(sock, str, strlen(str)+1, 0) < 0) perro("send");

		SEND_OR_RECEIVE = 0; // flag para identificar se é envio ou recebimento
        str[strcspn(str, "\n")] = 0;
		if (LOG_SAVE)
      		update_log(str, LOG_NAME, SEND_OR_RECEIVE);
    }
    kill(pid, SIGKILL);
    printf("\033[0m Goodbye.\n");  
}

void send_cmd_compressed(int sock, int pid) {
    char str[MAX_MSG_LENGTH];
    printf("\033[32m> ");
    while (fgets(str, MAX_MSG_LENGTH, stdin) == str) {
        if (strncmp(str, END_STRING, strlen(END_STRING)) == 0) break;
        unsigned long command_size = strlen(str) + 1;
        unsigned long compressed_size = compressBound(command_size);
        char *compressed_msg = (char *)malloc(compressed_size * sizeof(char));

        if (compress((Bytef *)compressed_msg, &compressed_size, (const Bytef *)str, command_size) != Z_OK) {
            printf("Erro ao comprimir o comando.\n");
            free(compressed_msg);
            continue;
        }

        // Envio do tamanho do buffer comprimido
        if (send(sock, &compressed_size, sizeof(unsigned long), 0) < 0) {
            perror("Erro ao enviar o tamanho do buffer comprimido");
            free(compressed_msg);
            continue;
        }

        // Envio dos dados comprimidos
        if (send(sock, compressed_msg, compressed_size, 0) < 0) {
            perror("Erro ao enviar os dados comprimidos");
            free(compressed_msg);
            continue;
        }

		SEND_OR_RECEIVE = 0; // flag para identificar se é envio ou recebimento

		if (LOG_SAVE)
      		update_log(str, LOG_NAME, SEND_OR_RECEIVE);
    }
    kill(pid, SIGKILL);
    printf("\033[0m Goodbye.\n");  
}

void receive(int sock) {
    char buf[MAX_MSG_LENGTH] = {0};
    int filled = 0;
    while (filled = recv(sock, buf, MAX_MSG_LENGTH-1, 0)) {
        buf[filled] = '\0';
        printf("\033[35m%s", buf);

		SEND_OR_RECEIVE = 1;

		if (LOG_SAVE)
      		update_log(buf, LOG_NAME, SEND_OR_RECEIVE);
        fflush(stdout);
		
    }
    printf("\033[0m Server disconnected.\n");  
}

int main(int argc, char **argv) {
  	// --port, --log, --host
    client_possible_flags( argc,  argv, &LOG_NAME, &PORT, &LOG_SAVE, &HOST, &COMPRESS);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) perro("socket");

    struct in_addr server_addr;
    server_addr.s_addr = inet_addr(HOST);

    struct sockaddr_in connection;
    connection.sin_family = AF_INET;
    connection.sin_addr = server_addr; 
    connection.sin_port = htons(PORT);
    if (connect(sock, (const struct sockaddr*) &connection, sizeof(connection)) != 0) perro("connect");

    int pid;
    if(COMPRESS){
        if (pid = fork())
            send_cmd_compressed(sock, pid);
        else
            receive(sock);
    }else{
    if (pid = fork())
        send_cmd(sock, pid);
    else
        receive(sock);
    }
    return 0;
}