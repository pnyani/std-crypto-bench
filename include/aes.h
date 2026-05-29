#ifndef AES_H
#define AES_H

/* NIST FIPS 197. */

#include <stddef.h>
#include <stdint.h>

#define AES_BLOCK_SIZE   16
#define AES128_KEY_SIZE  16
#define AES256_KEY_SIZE  32
#define AES128_NR        10
#define AES256_NR        14

typedef struct { uint32_t rk[44]; } aes128_ctx_t;
typedef struct { uint32_t rk[60]; } aes256_ctx_t;

void aes128_key_expand(const uint8_t key[16], aes128_ctx_t *ctx);
void aes128_encrypt(const uint8_t in[16], uint8_t out[16], const aes128_ctx_t *ctx);
void aes128_decrypt(const uint8_t in[16], uint8_t out[16], const aes128_ctx_t *ctx);
void aes128_encrypt_blocks(const uint8_t *in, uint8_t *out,
                           size_t blocks, const aes128_ctx_t *ctx);
void aes128_decrypt_blocks(const uint8_t *in, uint8_t *out,
                           size_t blocks, const aes128_ctx_t *ctx);

void aes256_key_expand(const uint8_t key[32], aes256_ctx_t *ctx);
void aes256_encrypt(const uint8_t in[16], uint8_t out[16], const aes256_ctx_t *ctx);
void aes256_decrypt(const uint8_t in[16], uint8_t out[16], const aes256_ctx_t *ctx);
void aes256_encrypt_blocks(const uint8_t *in, uint8_t *out,
                           size_t blocks, const aes256_ctx_t *ctx);
void aes256_decrypt_blocks(const uint8_t *in, uint8_t *out,
                           size_t blocks, const aes256_ctx_t *ctx);

#endif /* AES_H */
