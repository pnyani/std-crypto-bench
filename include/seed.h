#ifndef SEED_H
#define SEED_H

/* KS X 1213 / RFC 4269. */

#include <stddef.h>
#include <stdint.h>

#define SEED_BLOCK_SIZE 16
#define SEED_KEY_SIZE   16
#define SEED_NR         16

typedef struct {
    uint32_t ks[32];
} seed_ctx_t;

void seed_key_expand(const uint8_t key[16], seed_ctx_t *ctx);
void seed_encrypt(const uint8_t in[16], uint8_t out[16], const seed_ctx_t *ctx);
void seed_decrypt(const uint8_t in[16], uint8_t out[16], const seed_ctx_t *ctx);
void seed_encrypt_blocks(const uint8_t *in, uint8_t *out,
                         size_t blocks, const seed_ctx_t *ctx);
void seed_decrypt_blocks(const uint8_t *in, uint8_t *out,
                         size_t blocks, const seed_ctx_t *ctx);

#endif /* SEED_H */
