#ifndef AES_VARIANT_H
#define AES_VARIANT_H

/* Parameterized AES. Scenario A: Nk=4, variable Nr (non-standard). Scenario B: standard NIST sizes. */

#include <stddef.h>
#include <stdint.h>

#define AES_VARIANT_MAX_RK_WORDS 60

typedef struct {
    int Nk;
    int Nr;
} aes_params_t;

/* Scenario B */
extern const aes_params_t AES_PARAMS_128;   /* Nk=4, Nr=10 */
extern const aes_params_t AES_PARAMS_192;   /* Nk=6, Nr=12 */
extern const aes_params_t AES_PARAMS_256;   /* Nk=8, Nr=14 */

/* Scenario A */
extern const aes_params_t AES_PARAMS_128_R6;
extern const aes_params_t AES_PARAMS_128_R8;
extern const aes_params_t AES_PARAMS_128_R12;
extern const aes_params_t AES_PARAMS_128_R14;

void aes_variant_key_expand(const uint8_t *key,
                             uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                             const aes_params_t *p);

void aes_variant_encrypt(const uint8_t in[16], uint8_t out[16],
                         const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                         const aes_params_t *p);

void aes_variant_decrypt(const uint8_t in[16], uint8_t out[16],
                         const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                         const aes_params_t *p);

void aes_variant_encrypt_blocks(const uint8_t *in, uint8_t *out,
                                size_t blocks,
                                const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                                const aes_params_t *p);

void aes_variant_decrypt_blocks(const uint8_t *in, uint8_t *out,
                                size_t blocks,
                                const uint32_t w[AES_VARIANT_MAX_RK_WORDS],
                                const aes_params_t *p);

#endif /* AES_VARIANT_H */
