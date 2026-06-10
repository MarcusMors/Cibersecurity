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
    BIGNUM *d = BN_new();
    BIGNUM *M = BN_new();
    BIGNUM *S = BN_new();
    
    // Set keys from Task 2
    BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
    BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");
    
    // python3 -c 'print("I owe you $2000.".encode("utf-8").hex())'
    // 49206f776520796f752024323030302e
    // python3 -c 'print("I owe you $3000.".encode("utf-8").hex())'
    // 49206f776520796f752024333030302e
    BN_hex2bn(&M, "49206f776520796f752024323030302e");
    
    printf("=== Task 4: Signing a Message ===\n\n");
    printf("Message: I owe you $2000.\n");
    printBN("Message (hex) =", M);
    printf("\nPrivate Key:\n");
    printBN("d =", d);
    printBN("n =", n);
    
    // Generate signature: S = M^d mod n
    BN_mod_exp(S, M, d, n, ctx);
    printf("\nSignature (S):\n");
    printBN("S =", S);
    
    // Cleanup
    BN_free(n);
    BN_free(d);
    BN_free(M);
    BN_free(S);
    BN_CTX_free(ctx);
    
    return 0;
}