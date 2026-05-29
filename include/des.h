#ifndef DES_H
#define DES_H

/* FIPS 46-3 (withdrawn). 56-bit effective key; rk[] upper 16 bits unused. */

#include <stddef.h>
#include <stdint.h>

#define DES_BLOCK_SIZE 8
#define DES_KEY_SIZE   8
#define DES_NR         16

typedef struct {
    uint64_t rk[16];
} des_ctx_t;

void des_key_expand(const uint8_t key[8], des_ctx_t *ctx);
void des_encrypt(const uint8_t in[8], uint8_t out[8], const des_ctx_t *ctx);
void des_decrypt(const uint8_t in[8], uint8_t out[8], const des_ctx_t *ctx);
void des_encrypt_blocks(const uint8_t *in, uint8_t *out,
                        size_t blocks, const des_ctx_t *ctx);
void des_decrypt_blocks(const uint8_t *in, uint8_t *out,
                        size_t blocks, const des_ctx_t *ctx);

#endif /* DES_H */
