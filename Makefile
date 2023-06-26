all: clear server client

server: gcc server.c "server_utils/server_option.c" "client_utils/error_handling.c"   -o server
client: gcc client.c "client_utils/client_option.c" "client_utils/socket_options.c" "client_utils/log_option.c" "client_utils/host_options.c" "client_utils/error_handling.c"  -o client

clean: 
	rm -f server client
