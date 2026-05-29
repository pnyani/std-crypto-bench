#include "aes_internal.h"
#include "aes_variant.h"

const aes_params_t AES_PARAMS_128 = { 4, 10 };
const aes_params_t AES_PARAMS_192 = { 6, 12 };
const aes_params_t AES_PARAMS_256 = { 8, 14 };

const aes_params_t AES_PARAMS_128_R6  = { 4,  6 };
const aes_params_t AES_PARAMS_128_R8  = { 4,  8 };
const aes_params_t AES_PARAMS_128_R12 = { 4, 12 };
const aes_params_t AES_PARAMS_128_R14 = { 4, 14 };

void aes_variant_key_expand(const uint8_t *key,
                            uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                            const aes_params_t *p)
{
    aes_key_expand_generic(key, w, p->Nk, p->Nr);
}

void aes_variant_encrypt(const uint8_t in[16], uint8_t out[16],
                         const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                         const aes_params_t *p)
{
    aes_encrypt_generic(in, out, w, p->Nr);
}

void aes_variant_decrypt(const uint8_t in[16], uint8_t out[16],
                         const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                         const aes_params_t *p)
{
    aes_decrypt_generic(in, out, w, p->Nr);
}

void aes_variant_encrypt_blocks(const uint8_t *in, uint8_t *out,
                                size_t blocks,
                                const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                                const aes_params_t *p)
{
    size_t i;
    for (i = 0; i < blocks; i++) aes_variant_encrypt(in + 16*i, out + 16*i, w, p);
}

void aes_variant_decrypt_blocks(const uint8_t *in, uint8_t *out,
                                size_t blocks,
                                const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                                const aes_params_t *p)
{
    size_t i;
    for (i = 0; i < blocks; i++) aes_variant_decrypt(in + 16*i, out + 16*i, w, p);
}
