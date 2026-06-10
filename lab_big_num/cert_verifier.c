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
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>


#define SHA256_HASH_LEN 32
#define SHA256_DER_LEN 19

// Der means Distinguished Encoding Rules
// it's a strict, unambiguous binary encoding format for ASN.1 data structures.
// Purpose: When verifying, the code extracts the actual hash by skipping past this known DER prefix to compare against the independently computed hash of the certificate body.
const unsigned char sha256_der[SHA256_DER_LEN] = {
    0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
    0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20
};

void printBN(const char *msg, const BIGNUM *a)
{
    char *str = BN_bn2hex(a);
    printf("%s %s\n", msg, str);
    OPENSSL_free(str);
}

int read_hex_file(const char *filename, BIGNUM *bn)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char buf[8192];
    int ret = fscanf(fp, "%s", buf);
    fclose(fp);
    return ret == 1 && BN_hex2bn(&bn, buf) > 0;
}

int read_hash_file(const char *filename, unsigned char *hash, int len)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char buf[256];
    int ret = fscanf(fp, "%s", buf);
    fclose(fp);
    if (ret != 1 || strlen(buf) != len*2) return 0;
    for (int i=0; i<len; i++) sscanf(buf+i*2, "%2hhx", &hash[i]);
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s n.hex e.hex signature.hex hash.hex\n", argv[0]);
        return 1;
    }

    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *n = BN_new(), *e = BN_new(), *S = BN_new(), *M = BN_new();
    unsigned char expected_hash[SHA256_HASH_LEN], extracted_hash[SHA256_HASH_LEN];

    if (!read_hex_file(argv[1], n) || !read_hex_file(argv[2], e) || 
        !read_hex_file(argv[3], S) || !read_hash_file(argv[4], expected_hash, SHA256_HASH_LEN)) {
        fprintf(stderr, "Error reading input files\n");
        return 1;
    }

    // Decrypt signature
    BN_mod_exp(M, S, e, n, ctx);
    
    // Extract padded message
    int m_len = BN_num_bytes(M);
    unsigned char *m_buf = malloc(m_len);
    BN_bn2bin(M, m_buf);

    // Find PKCS#1 v1.5 padding separator
    int sep = -1;
    for (int i=2; i<m_len-SHA256_DER_LEN-SHA256_HASH_LEN; i++) {
        if (m_buf[i] == 0x00 && m_buf[0]==0x00 && m_buf[1]==0x01) {
            int valid = 1;
            for (int j=2; j<i; j++) if (m_buf[j] != 0xFF) { valid=0; break; }
            if (valid && i-2 >= 8) { sep = i; break; }
        }
    }

    if (sep==-1 || m_len-sep-1 != SHA256_DER_LEN+SHA256_HASH_LEN ||
        memcmp(m_buf+sep+1, sha256_der, SHA256_DER_LEN)) {
        printf("✗ Invalid signature format\n");
        return 1;
    }

    memcpy(extracted_hash, m_buf+sep+1+SHA256_DER_LEN, SHA256_HASH_LEN);
    
    printf("Expected hash:  ");
    for (int i=0; i<SHA256_HASH_LEN; i++) printf("%02X", expected_hash[i]);
    printf("\nExtracted hash: ");
    for (int i=0; i<SHA256_HASH_LEN; i++) printf("%02X", extracted_hash[i]);
    printf("\n");

    if (!memcmp(extracted_hash, expected_hash, SHA256_HASH_LEN))
        printf("\n✓ Signature VALID. Certificate verified.\n");
    else
        printf("\n✗ Signature INVALID. Verification failed.\n");

    free(m_buf);
    BN_free(n); BN_free(e); BN_free(S); BN_free(M);
    BN_CTX_free(ctx);
    return 0;
}