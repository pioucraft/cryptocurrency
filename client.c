int client_input_handler(char* input_str) {
    char* buffer = malloc((strlen(input_str) + 1) * sizeof(char));
    memcpy(buffer, input_str, strlen(input_str) + 1);

    char* receiver = strtok(buffer, " ");
    char* amount_str = strtok(NULL, " ");
    int amount = atoi(amount_str);
    printf("Sending %d to %s\n", amount, receiver);

    free(buffer);
}
