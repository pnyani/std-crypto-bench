#ifndef TDES_H
#define TDES_H

/* NIST SP 800-67 Rev. 2 (withdrawn). EDE: E_K3(D_K2(E_K1(P))). */

#include <stddef.h>
#include <stdint.h>
#include "des.h"

#define TDES_BLOCK_SIZE    8
#define TDES_KEY_SIZE_2KEY 16
#define TDES_KEY_SIZE_3KEY 24

typedef struct {
    des_ctx_t k1;
    des_ctx_t k2;
    des_ctx_t k3;
} tdes_ctx_t;

/* 3-key EDE: key[0..7]=K1, key[8..15]=K2, key[16..23]=K3 */
void tdes3_key_expand(const uint8_t key[24], tdes_ctx_t *ctx);
void tdes3_encrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx);
void tdes3_decrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx);
void tdes3_encrypt_blocks(const uint8_t *in, uint8_t *out,
                          size_t blocks, const tdes_ctx_t *ctx);
void tdes3_decrypt_blocks(const uint8_t *in, uint8_t *out,
                          size_t blocks, const tdes_ctx_t *ctx);

/* 2-key EDE: key[0..7]=K1, key[8..15]=K2, K3=K1 */
void tdes2_key_expand(const uint8_t key[16], tdes_ctx_t *ctx);
void tdes2_encrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx);
void tdes2_decrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx);
void tdes2_encrypt_blocks(const uint8_t *in, uint8_t *out,
                          size_t blocks, const tdes_ctx_t *ctx);
void tdes2_decrypt_blocks(const uint8_t *in, uint8_t *out,
                          size_t blocks, const tdes_ctx_t *ctx);

#endif /* TDES_H */
