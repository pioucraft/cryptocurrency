#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int port = 53565;

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        if(strncmp(argv[i], "-p", 2) | strncmp(argv[i], "--port", 6)) {
            i++;
            if(i == argc) {
                printf("Error, please specify parameter value for port (-p/--port)\n");
                return 1;
            }
            port = atoi(argv[i]);
        }
    }
    printf("Started listening on port : %d\n", port);
    return 0;
}

