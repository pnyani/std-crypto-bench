/* DES: NIST SP 800-17 Table 1 KAT. 3DES: K1=K2=K3 degenerate check. */

#include <stdint.h>
#include "test_utils.h"
#include "des.h"
#include "3des.h"

void run_des_tests(void)
{
    uint8_t out[8];

    {
        des_ctx_t ctx;
        static const uint8_t key[8] = {
            0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01
        };
        static const uint8_t pt[8] = {
            0x80,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
        };
        static const uint8_t ct[8] = {
            0x95,0xf8,0xa5,0xe5, 0xdd,0x31,0xd9,0x00
        };

        des_key_expand(key, &ctx);
        des_encrypt(pt, out, &ctx);
        test_assert_bytes("DES NIST SP800-17 T1 V1 encrypt", ct, out, 8);
        des_decrypt(ct, out, &ctx);
        test_assert_bytes("DES NIST SP800-17 T1 V1 decrypt", pt, out, 8);
    }

    /* E_K3(D_K2(E_K1(P))) with K1=K2=K3=K reduces to E_K(P). */
    {
        des_ctx_t   des_ctx;
        tdes_ctx_t  tdes_ctx;
        uint8_t     des_out[8], tdes_out[8];

        static const uint8_t key[8] = {
            0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01
        };
        uint8_t tdes_key[24];
        int i;
        for (i = 0; i < 8; i++) {
            tdes_key[i]    = key[i];
            tdes_key[i+8]  = key[i];
            tdes_key[i+16] = key[i];
        }

        static const uint8_t pt[8] = {
            0x80,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
        };

        des_key_expand(key, &des_ctx);
        des_encrypt(pt, des_out, &des_ctx);

        tdes3_key_expand(tdes_key, &tdes_ctx);
        tdes3_encrypt(pt, tdes_out, &tdes_ctx);

        test_assert_bytes("3DES K1=K2=K3 == DES (encrypt)", des_out, tdes_out, 8);

        des_decrypt(des_out, des_out, &des_ctx);
        tdes3_decrypt(tdes_out, tdes_out, &tdes_ctx);

        test_assert_bytes("3DES K1=K2=K3 == DES (decrypt)", pt, tdes_out, 8);
    }
}
