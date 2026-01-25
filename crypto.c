EVP_PKEY* load_key(const char* file, int pub) {  // Params: filename, 1=pub/0=priv
    // start by opening the file and reading the contents
    FILE* key_file = fopen(file, "r");
    if (!key_file) {
        perror("Error opening key file");
        return NULL;
    }
    char buffer[4096];
    size_t read_bytes = fread(buffer, 1, sizeof(buffer), key_file);
    fclose(key_file);
    if (read_bytes == 0) {
        fprintf(stderr, "Error reading key file or file is empty\n");
        return NULL;
    }
    buffer[read_bytes] = '\0';  // Null-terminate the buffer
}

