/**
 * Copyright (C) 2025 José Enrique Vilca Campana
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
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
    
    // Initialize BIGNUMs
    BIGNUM *n = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *S = BN_new();
    BIGNUM *M = BN_new();
    BIGNUM *M_verify = BN_new();
    
    // Set Alice's public key
    BN_hex2bn(&n, "AE1CD4DC432798D933779FBD46C6E1247F0CF1233595113AA51B450F18116115");
    BN_hex2bn(&e, "010001");
    
    // Set signature
    BN_hex2bn(&S, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6802F");
    // BN_hex2bn(&S, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6803F");
    
    // Set original message: "Launch a missile."
    // python3 -c 'print("Launch a missile.".encode("utf-8").hex())'
    BN_hex2bn(&M, "4C61756E63682061206D697373696C652E");
    
    printf("=== Task 5: Verifying a Signature ===\n\n");
    printf("Message: Launch a missile.\n");
    printBN("M (hex) =", M);
    printf("\nSignature:\n");
    printBN("S =", S);
    printf("\nPublic Key:\n");
    printBN("e =", e);
    printBN("n =", n);
    
    // Verify: M_verify = S^e mod n
    BN_mod_exp(M_verify, S, e, n, ctx);
    printBN("\nDecrypted Signature =", M_verify);
    
    // Check validity
    if (BN_cmp(M, M_verify) == 0) {
        printf("\n✓ Signature VALID. Message is from Alice.\n");
    } else {
        printf("\n✗ Signature INVALID. Message is NOT from Alice.\n");
    }
    
    // Cleanup
    BN_free(n);
    BN_free(e);
    BN_free(S);
    BN_free(M);
    BN_free(M_verify);
    BN_CTX_free(ctx);
    
    return 0;
}