# Generating OpenSSL keys :
```sh
openssl genpkey -algorithm RSA -out private.pem -pkeyopt rsa_keygen_bits:2048
```

# Compiling :
```sh
gcc main.c -lssl -lcrypto
```
