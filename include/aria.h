#ifndef ARIA_H
#define ARIA_H

/* KS X 1213-1 / RFC 5794. */

#include <stddef.h>
#include <stdint.h>

#define ARIA_BLOCK_SIZE   16
#define ARIA128_KEY_SIZE  16
#define ARIA128_NR        12

typedef struct {
    uint8_t rk[13][16];
    uint8_t dk[13][16];
} aria128_ctx_t;

void aria128_key_expand(const uint8_t key[16], aria128_ctx_t *ctx);
void aria128_encrypt(const uint8_t in[16], uint8_t out[16], const aria128_ctx_t *ctx);
void aria128_decrypt(const uint8_t in[16], uint8_t out[16], const aria128_ctx_t *ctx);
void aria128_encrypt_blocks(const uint8_t *in, uint8_t *out,
                            size_t blocks, const aria128_ctx_t *ctx);
void aria128_decrypt_blocks(const uint8_t *in, uint8_t *out,
                            size_t blocks, const aria128_ctx_t *ctx);

#endif /* ARIA_H */
