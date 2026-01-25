#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "crypto.c"
#include "client.c"

#define MAX_NODES_LENGTH 128

int port = 53565;
int nodes_length = 0;
char* nodes[MAX_NODES_LENGTH];
char* nodes_str;
char* private_key_path = "private.pem";

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        if(strncmp(argv[i], "-p", 2) == 0 || strncmp(argv[i], "--port", 6) == 0) {
            i++;
            if(i == argc) {
                printf("Error, please specify parameter value for port (-p/--port)\n");
                return 1;
            }
            port = atoi(argv[i]);
        }
        else if(strncmp(argv[i], "-n", 2) == 0 || strncmp(argv[i], "--nodes", 7) == 0) {
            i++;
            if(i == argc) {
                printf("Error, please specify parameter value for nodes (-n/--nodes)\n");
                return 1;
            }
            nodes_str = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(nodes_str, argv[i]);

            nodes[0] = strtok(nodes_str, " ");
            for(int j = 1; j < MAX_NODES_LENGTH; j++) {
                char* strtoked = strtok(NULL, " ");
                nodes[j] = strtoked ? strtoked : " ";
            }
        }
        else if(strncmp(argv[i], "-k", 2) == 0 || strncmp(argv[i], "--key", 5) == 0) {
            i++;
            if(i == argc) {
                printf("Error, please specify parameter value for private key path (-k/--key)\n");
                return 1;
            }
            private_key_path = argv[i];
        }
    }

    // load private key
    EVP_PKEY* private_key = load_key(private_key_path, 0);


    // 1. Create a socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error creating socket");
        return 1;
    }

    // 2. Define the server address
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    // 3. Bind the socket to the port and address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error binding socket");
        close(server_fd);
        return 1;
    }

    // 4. Start listening for incoming connections
    if (listen(server_fd, 500) < 0) {
        perror("Error listening on socket");
        close(server_fd);
        return 1;
    }

    printf("Started listening on port : %d\n", port);
    printf("Usage : '<receiver> <amout>\n");

    int stdin_fd = 0;

    fd_set fds;
    int max_fd = (stdin_fd > server_fd ? stdin_fd : server_fd) + 1;


    while(1) {
        FD_ZERO(&fds);
        FD_SET(stdin_fd, &fds);
        FD_SET(server_fd, &fds);
        select(max_fd, &fds, NULL, NULL, NULL);

        if (FD_ISSET(stdin_fd, &fds)) {
            char buf[1024];
            ssize_t n = read(stdin_fd, buf, sizeof(buf) - 1);
            if (n > 0) {
                buf[n] = '\0';
                client_input_handler(buf);
            }
        }
        if (FD_ISSET(server_fd, &fds)) {

            int client_fd = accept(server_fd, NULL, NULL);

            char* response = "hello";
            send(client_fd, response, strlen(response), 0);
            close(client_fd);
        }
    }

    free(nodes_str);
    return 0;
}


