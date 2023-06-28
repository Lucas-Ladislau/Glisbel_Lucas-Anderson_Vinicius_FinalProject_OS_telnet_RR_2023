all: clear server client

server: gcc server.c "server_utils/server_option.c" "client_utils/error_handling.c" "client_utils/host_options.c" "client_utils/socket_options.c" "compress_utils/compress_functions.c" -lz -o server
client: gcc client.c "client_utils/client_option.c" "client_utils/socket_options.c" "client_utils/log_option.c" "client_utils/host_options.c" "compress_utils/compress_functions.c" "client_utils/error_handling.c" -lz -o client

clean: 
	rm -f server client
