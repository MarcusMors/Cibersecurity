#include <stdio.h>
#include <stdlib.h>
#include <openssl/bn.h>

void printBN(const char *msg, const BIGNUM *a)
{
   char *number_str = BN_bn2hex(a);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}

int main()
{
    BN_CTX *ctx = BN_CTX_new();
    
    // Initialize BIGNUMs for keys and message
    BIGNUM *n = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *M = BN_new();
    BIGNUM *C = BN_new();
    BIGNUM *M_verify = BN_new();
    
    // Set public key (n, e) and private key (d) from hex
    BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
    BN_hex2bn(&e, "010001");  // 65537 in decimal
    BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");
    
    // Set message M: "A top secret!" as hex (4120746f702073656372657421)
    BN_hex2bn(&M, "4120746f702073656372657421");
    
    printf("=== Task 2: Encrypting a Message ===\n\n");
    printf("Public Key:\n");
    printBN("n =", n);
    printBN("e =", e);
    
    printf("\nOriginal Message (M):\n");
    printBN("M =", M);
    
    // Encrypt: C = M^e mod n
    BN_mod_exp(C, M, e, n, ctx);
    printf("\nEncrypted Message (C):\n");
    printBN("C =", C);
    
    // Decrypt to verify: M_verify = C^d mod n
    BN_mod_exp(M_verify, C, d, n, ctx);
    printf("\nDecrypted Message (M_verify):\n");
    printBN("M_verify =", M_verify);
    
    // Verify correctness
    if (BN_cmp(M, M_verify) == 0) {
        printf("\n✓ Success! Decrypted message matches original.\n");
    } else {
        printf("\n✗ Error! Decryption failed.\n");
    }
    
    // ===== Task 3: Decrypting a Message =====
    printf("\n\n=== Task 3: Decrypting a Message ===\n\n");
    
    // Set ciphertext C
    BIGNUM *C_task3 = BN_new();
    BN_hex2bn(&C_task3, "8C0F971DF2F3672B28811407E2DABBE1DA0FEBBBDFC7DCB67396567EA1E2493F");
    
    // Decrypt: M_task3 = C_task3^d mod n
    BIGNUM *M_task3 = BN_new();
    BN_mod_exp(M_task3, C_task3, d, n, ctx);
    
    printBN("Ciphertext C =", C_task3);
    printBN("Decrypted Message (hex) =", M_task3);
    
    // Convert decrypted BIGNUM to ASCII string
    int msg_len = BN_num_bytes(M_task3);
    unsigned char *ascii_msg = (unsigned char *)malloc(msg_len + 1); // +1 for null terminator
    
    if (ascii_msg != NULL) {
        BN_bn2bin(M_task3, ascii_msg);
        ascii_msg[msg_len] = '\0'; // Add null terminator
        
        printf("Decrypted Message (ASCII) = %s\n", ascii_msg);
        
        free(ascii_msg);
    } else {
        printf("Error: Memory allocation failed\n");
    }
    
    // Cleanup Task 3
    BN_free(C_task3);
    BN_free(M_task3);
    
    // Cleanup Task 2
    BN_free(n);
    BN_free(e);
    BN_free(d);
    BN_free(M);
    BN_free(C);
    BN_free(M_verify);
    BN_CTX_free(ctx);
    
    return 0;
}