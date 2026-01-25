int sign_message(const char* priv_file, const char* message, unsigned char* signature) {
    FILE* priv_fd = fopen(priv_file, "r");
    if (!priv_fd) {
        printf("Error opening private key file\n");
        return 0;
    }
    
    EVP_PKEY* pkey = PEM_read_PrivateKey(priv_fd, NULL, NULL, NULL);
    fclose(priv_fd);
    
    if (!pkey) {
        printf("Error reading private key\n");
        ERR_print_errors_fp(stderr);
        return 0;
    }
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx || !EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, pkey)) {
        printf("Error initializing signing\n");
        EVP_PKEY_free(pkey);
        return 0;
    }
    
    size_t msg_len = strlen(message);
    if (!EVP_DigestSignUpdate(ctx, message, msg_len)) {
        printf("Error updating signature\n");
        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return 0;
    }
    
    size_t sig_len = SIG_LEN;
    if (!EVP_DigestSignFinal(ctx, signature, &sig_len)) {
        printf("Error finalizing signature\n");
        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return 0;
    }
    
    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    printf("Signature created successfully (%zu bytes)\n", sig_len);
    return 1;
}

