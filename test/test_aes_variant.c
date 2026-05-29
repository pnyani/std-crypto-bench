/* Scenario B: NIST FIPS 197 C.1/C.2/C.3. Scenario A: round-trip only (no standard vectors). */

#include <stdint.h>
#include <string.h>
#include "test_utils.h"
#include "aes_variant.h"

void run_aes_variant_tests(void)
{
    uint32_t w[AES_VARIANT_MAX_RK_WORDS];
    uint8_t  out[16], rt[16];

    /* AES-128 C.1 */
    {
        static const uint8_t key[16] = {
            0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
            0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f
        };
        static const uint8_t pt[16] = {
            0x00,0x11,0x22,0x33, 0x44,0x55,0x66,0x77,
            0x88,0x99,0xaa,0xbb, 0xcc,0xdd,0xee,0xff
        };
        static const uint8_t ct[16] = {
            0x69,0xc4,0xe0,0xd8, 0x6a,0x7b,0x04,0x30,
            0xd8,0xcd,0xb7,0x80, 0x70,0xb4,0xc5,0x5a
        };

        aes_variant_key_expand(key, w, &AES_PARAMS_128);
        aes_variant_encrypt(pt, out, w, &AES_PARAMS_128);
        test_assert_bytes("AES_PARAMS_128 C1 encrypt", ct, out, 16);
        aes_variant_decrypt(ct, out, w, &AES_PARAMS_128);
        test_assert_bytes("AES_PARAMS_128 C1 decrypt", pt, out, 16);
    }

    /* AES-192 C.2 */
    {
        static const uint8_t key[24] = {
            0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
            0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f,
            0x10,0x11,0x12,0x13, 0x14,0x15,0x16,0x17
        };
        static const uint8_t pt[16] = {
            0x00,0x11,0x22,0x33, 0x44,0x55,0x66,0x77,
            0x88,0x99,0xaa,0xbb, 0xcc,0xdd,0xee,0xff
        };
        static const uint8_t ct[16] = {
            0xdd,0xa9,0x7c,0xa4, 0x86,0x4c,0xdf,0xe0,
            0x6e,0xaf,0x70,0xa0, 0xec,0x0d,0x71,0x91
        };

        aes_variant_key_expand(key, w, &AES_PARAMS_192);
        aes_variant_encrypt(pt, out, w, &AES_PARAMS_192);
        test_assert_bytes("AES_PARAMS_192 C2 encrypt", ct, out, 16);
        aes_variant_decrypt(ct, out, w, &AES_PARAMS_192);
        test_assert_bytes("AES_PARAMS_192 C2 decrypt", pt, out, 16);
    }

    /* AES-256 C.3 */
    {
        static const uint8_t key[32] = {
            0x60,0x3d,0xeb,0x10, 0x15,0xca,0x71,0xbe,
            0x2b,0x73,0xae,0xf0, 0x85,0x7d,0x77,0x81,
            0x1f,0x35,0x2c,0x07, 0x3b,0x61,0x08,0xd7,
            0x2d,0x98,0x10,0xa3, 0x09,0x14,0xdf,0xf4
        };
        static const uint8_t pt[16] = {
            0x00,0x11,0x22,0x33, 0x44,0x55,0x66,0x77,
            0x88,0x99,0xaa,0xbb, 0xcc,0xdd,0xee,0xff
        };
        static const uint8_t ct[16] = {
            0x8e,0xa2,0xb7,0xca, 0x51,0x67,0x45,0xbf,
            0xea,0xfc,0x49,0x90, 0x4b,0x49,0x60,0x89
        };

        aes_variant_key_expand(key, w, &AES_PARAMS_256);
        aes_variant_encrypt(pt, out, w, &AES_PARAMS_256);
        test_assert_bytes("AES_PARAMS_256 C3 encrypt", ct, out, 16);
        aes_variant_decrypt(ct, out, w, &AES_PARAMS_256);
        test_assert_bytes("AES_PARAMS_256 C3 decrypt", pt, out, 16);
    }

    /* Scenario A: non-standard Nr, round-trip only */
    {
        static const uint8_t key[16] = {
            0x2b,0x7e,0x15,0x16, 0x28,0xae,0xd2,0xa6,
            0xab,0xf7,0x15,0x88, 0x09,0xcf,0x4f,0x3c
        };
        static const uint8_t pt[16] = {
            0x32,0x43,0xf6,0xa8, 0x88,0x5a,0x30,0x8d,
            0x31,0x31,0x98,0xa2, 0xe0,0x37,0x07,0x34
        };
        const aes_params_t *params[] = {
            &AES_PARAMS_128_R6,
            &AES_PARAMS_128_R8,
            &AES_PARAMS_128_R12,
            &AES_PARAMS_128_R14
        };
        const char *labels[] = {
            "AES_PARAMS_128_R6 round-trip",
            "AES_PARAMS_128_R8 round-trip",
            "AES_PARAMS_128_R12 round-trip",
            "AES_PARAMS_128_R14 round-trip"
        };
        int n = (int)(sizeof(params) / sizeof(params[0]));
        int i;

        for (i = 0; i < n; i++) {
            aes_variant_key_expand(key, w, params[i]);
            aes_variant_encrypt(pt, out, w, params[i]);
            aes_variant_decrypt(out, rt,  w, params[i]);
            test_assert_bytes(labels[i], pt, rt, 16);
        }
    }
}
