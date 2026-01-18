#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NODES_LENGTH 128

int port = 53565;
int nodes_length = 0;
char* nodes[MAX_NODES_LENGTH];
char* nodes_str;

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
            nodes_str = malloc(strlen((argv[i]) + 1) * sizeof(char));
            strcpy(nodes_str, argv[i]);

            nodes[0] = strtok(nodes_str, " ");
            for(int j = 1; j < MAX_NODES_LENGTH; j++) {
                char* strtoked = strtok(NULL, " ");
                nodes[j] = strtoked ? strtoked : " ";

                printf("%s\n", nodes[j - 1]);
            }
        }
    }
    printf("Started listening on port : %d\n", port);

    free(nodes_str);
    return 0;
}

