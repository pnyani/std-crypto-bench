#include "3des.h"

void tdes3_key_expand(const uint8_t key[24], tdes_ctx_t *ctx)
{
    des_key_expand(key, &ctx->k1);
    des_key_expand(key + 8, &ctx->k2);
    des_key_expand(key + 16, &ctx->k3);
}

void tdes3_encrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx)
{
    uint8_t t1[8], t2[8];
    des_encrypt(in, t1, &ctx->k1);
    des_decrypt(t1, t2, &ctx->k2);
    des_encrypt(t2, out, &ctx->k3);
}

void tdes3_decrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx)
{
    uint8_t t1[8], t2[8];
    des_decrypt(in, t1, &ctx->k3);
    des_encrypt(t1, t2, &ctx->k2);
    des_decrypt(t2, out, &ctx->k1);
}

void tdes3_encrypt_blocks(const uint8_t *in, uint8_t *out, size_t blocks, const tdes_ctx_t *ctx)
{
    size_t i;
    for (i = 0; i < blocks; i++) tdes3_encrypt(in + 8*i, out + 8*i, ctx);
}

void tdes3_decrypt_blocks(const uint8_t *in, uint8_t *out, size_t blocks, const tdes_ctx_t *ctx)
{
    size_t i;
    for (i = 0; i < blocks; i++) tdes3_decrypt(in + 8*i, out + 8*i, ctx);
}

void tdes2_key_expand(const uint8_t key[16], tdes_ctx_t *ctx)
{
    des_key_expand(key, &ctx->k1);
    des_key_expand(key + 8, &ctx->k2);
    des_key_expand(key, &ctx->k3);
}

void tdes2_encrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx)
{
    tdes3_encrypt(in, out, ctx);
}

void tdes2_decrypt(const uint8_t in[8], uint8_t out[8], const tdes_ctx_t *ctx)
{
    tdes3_decrypt(in, out, ctx);
}

void tdes2_encrypt_blocks(const uint8_t *in, uint8_t *out, size_t blocks, const tdes_ctx_t *ctx)
{
    tdes3_encrypt_blocks(in, out, blocks, ctx);
}

void tdes2_decrypt_blocks(const uint8_t *in, uint8_t *out, size_t blocks, const tdes_ctx_t *ctx)
{
    tdes3_decrypt_blocks(in, out, blocks, ctx);
}
