#include "mt.h"
#include "server_utils/server_option.h"
#include "compress_utils/compress_functions.h"
#include <stdio.h>
#include <stdlib.h>


#define D(...) fprintf(new_stream, __VA_ARGS__)

char *HOST = "127.0.0.1";
int PORT = 5555;
int COMPRESS = 0;

int main(int argc, char **argv) {
	server_possible_flags( argc,  argv, &PORT, &HOST, &COMPRESS);
	int sock;
	struct sockaddr_in server;
	char buf[MAX_MSG_LENGTH];
	char output[MAX_MSG_LENGTH];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) perro("opening socket");

	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(HOST);
	if(bind(sock, (void*) &server, sizeof(server))) perro("binding tcp socket");
	if(listen(sock, 1) == -1) perro("listen");
	
	struct sockaddr cli_addr;
	int cli_len = sizeof(cli_addr);
	int new_socket, new_fd, pid;
	FILE* new_stream;
	
	if(new_fd = dup(STDERR_FILENO) == -1) perro("dup");
	new_stream = fdopen(new_fd, "w");
	setbuf(new_stream, NULL); // sin buffering
	
	D("Initializing server...\n");
	while(new_socket = accept(sock, &cli_addr, &cli_len)) {
		D("Client connected.\nForking... ");
		if(pid = fork()) D("child pid = %d.\n", pid);
		else {
			pid = getpid();
			if(new_socket < 0) perro("accept");
			if(dup2(new_socket, STDOUT_FILENO) == -1) perro("dup2");
			if(dup2(new_socket, STDERR_FILENO) == -1) perro("dup2");
			D("\t[%d] Communication established.\n", pid);
			while(1) {
				if(COMPRESS){
					unsigned long compressed_size = 0;

					// Receber o tamanho do buffer comprimido
					if (recv(new_socket, &compressed_size, sizeof(unsigned long), 0) < 0) {
						perror("Erro ao receber o tamanho do buffer comprimido");
						continue;
					}

					// Alocação de buffer para os dados comprimidos
					char *compressed_msg = (char *)malloc(compressed_size * sizeof(char));

					// Receber os dados comprimidos
					if (recv(new_socket, compressed_msg, compressed_size, 0) < 0) {
						perror("Erro ao receber os dados comprimidos");
						free(compressed_msg);
						continue;
					}

					// Descompressão do comando
					unsigned long command_size = MAX_MSG_LENGTH; // Tamanho máximo esperado para o comando descomprimido
					char *uncompressed_msg = uncompress_buffer(compressed_msg, command_size, compressed_size);
					//free(compressed_msg);

					// Executar o comando recebido
					printf("[%d] Command received: %s\n", pid, uncompressed_msg);
					system(uncompressed_msg);
					
					printf("[%d] Finished executing command.\n", pid);
					
				}else{
					int readc = 0, filled = 0;
					while(1) {
						readc = recv(new_socket, buf+filled, MAX_MSG_LENGTH-filled-1, 0);
						if(!readc) break;
						filled += readc;
						if(buf[filled-1] == '\0') break;
					}
					if(!readc) {
						D("\t[%d] Client disconnected.\n", pid);
						break;
					}
						D("\t[%d] Command received: %s", pid, buf);
						system(buf);
						D("\t[%d] Finished executing command.\n", pid);
				}
					send(new_socket, "> ", 3, MSG_NOSIGNAL);
				
				
			}
			close(new_socket);
			D("\t[%d] Dying.", pid);
			exit(0);
		}
	}
	fclose(new_stream);
	close(sock);
	return 0;
}