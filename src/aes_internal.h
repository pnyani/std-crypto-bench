#ifndef AES_INTERNAL_H
#define AES_INTERNAL_H

/* Shared internals between aes.c and aes_variant.c. Not for external use. */

#include <stdint.h>

extern const uint8_t aes_sbox[256];
extern const uint8_t aes_inv_sbox[256];
extern const uint32_t aes_rcon[16];

uint32_t aes_load_be32(const uint8_t b[4]);
void aes_store_be32(uint8_t b[4], uint32_t v);
uint32_t aes_sub_word(uint32_t w);
uint32_t aes_rot_word(uint32_t w);
void aes_key_expand_generic(const uint8_t *key, uint32_t *w, int Nk, int Nr);
void aes_encrypt_generic(const uint8_t in[16], uint8_t out[16], const uint32_t *w, int Nr);
void aes_decrypt_generic(const uint8_t in[16], uint8_t out[16], const uint32_t *w, int Nr);

#endif /* AES_INTERNAL_H */
