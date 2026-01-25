#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#define MAX_MSG_LEN 1024
#define SIG_LEN 256

// Sign message with private key
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

// Verify signature with public key - FIXED VERSION
int verify_signature(const char* pub_file, const char* message, const unsigned char* signature, size_t sig_len) {
    FILE* pub_fd = fopen(pub_file, "r");
    if (!pub_fd) {
        printf("Error opening public key file\n");
        return 0;
    }
    
    // Fixed: Use PEM_read_PUBKEY instead of PEM_read_PublicKey
    EVP_PKEY* pkey = PEM_read_PUBKEY(pub_fd, NULL, NULL, NULL);
    fclose(pub_fd);
    
    if (!pkey) {
        printf("Error reading public key\n");
        ERR_print_errors_fp(stderr);
        return 0;
    }
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx || !EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey)) {
        printf("Error initializing verification\n");
        EVP_PKEY_free(pkey);
        return 0;
    }
    
    size_t msg_len = strlen(message);
    if (!EVP_DigestVerifyUpdate(ctx, message, msg_len)) {
        printf("Error updating verification\n");
        EVP_MD_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return 0;
    }
    
    int result = EVP_DigestVerifyFinal(ctx, signature, sig_len);
    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    
    if (result == 1) {
        printf("Signature is VALID\n");
    } else {
        printf("Signature is INVALID\n");
    }
    return result == 1;
}

int main() {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    char message[MAX_MSG_LEN];
    char verify_msg[MAX_MSG_LEN];
    unsigned char signature[SIG_LEN];
    
    printf("Enter message to sign: ");
    fgets(message, MAX_MSG_LEN, stdin);
    message[strcspn(message, "\n")] = 0;
    
    // Sign the message
    if (!sign_message("private.pem", message, signature)) {
        return 1;
    }
    
    printf("\nEnter message to verify against signature: ");
    fgets(verify_msg, MAX_MSG_LEN, stdin);
    verify_msg[strcspn(verify_msg, "\n")] = 0;
    
    // Verify signature - pass actual signature length
    verify_signature("public.pem", verify_msg, signature, SIG_LEN);
    
    return 0;
}

