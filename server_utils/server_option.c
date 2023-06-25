#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int port_option(int argc, char **argv) {
    int port = -1;
    int option;

    struct option long_options[] = {
        {"port", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };

    int option_index = 0;
    while ((option = getopt_long(argc, argv, "p:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s --port=portnumber\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (port == -1) {
        fprintf(stderr, "Please provide the port using --port option.\n");
        exit(EXIT_FAILURE);
    }

    return port;
}